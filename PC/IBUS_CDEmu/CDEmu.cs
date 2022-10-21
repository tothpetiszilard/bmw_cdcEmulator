using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.IO.Ports;

namespace IBUS_CDEmu
{
    public partial class CDEmu : Form
    {
        string[] portlist = null;
        SerialPort com = new SerialPort();
        int err_tmr_cnt;
        bool pollReceived_b = false;        
        const byte myID_cu8 = 0x18;
        CDEmu_StatusType globalStatus = CDEmu_StatusType.STOPPED;
        byte CDEmu_currentDiskId_u8 = 1;
        byte CDEmu_currentTrackId_u8 = 1;
        enum CDEmu_StatusType
        {
            STOPPED,
            STARTING,
            STARTED,
            FASTFORWARD,
            FASTBACKWARD,
            STOPPING,
            SEEKING
        }

        public CDEmu()
        {
            InitializeComponent();
            portlist = SerialPort.GetPortNames();
            port_cb.Text = "No Serial";
            port_cb.DataSource = portlist;
            CDEmu.CheckForIllegalCrossThreadCalls = false;
            status_l.Text = "STOPPED";
        }

        private byte CalcChecksum(byte [] inputData_au8)
        {
            byte crc_u8 = 0;
            for (int i = 0; i < inputData_au8.Length; i++)
            {
                crc_u8 ^= inputData_au8[i];
            }
            return crc_u8;
        }

        private void serial_connect()
        {
            if (!(com.IsOpen))
            {
                string portname = null;
                portname = port_cb.Text;
                if (!(portname == "No Serial"))
                {
                    timer1.Stop();
                    com.PortName = portname;
                    com.BaudRate = 9600;
                    com.ReadTimeout = 2000;
                    com.WriteTimeout = 2000;
                    com.Parity = Parity.Even;
                    try
                    {
                        com.Open();
                    }
                    catch (Exception e)
                    {
                        MessageBox.Show("Cannot open serial port. Error: " + e.Message);
                    }
                    if (com.IsOpen)
                    {
                        com.DtrEnable = true;
                        com.RtsEnable = true;
                        com.DataReceived += new SerialDataReceivedEventHandler(com_DataReceived);
                        port_cb.Enabled = false;
                        connect_b.Text = "Disconnect";
                        timer2.Start();
                        timer3.Start();
                        SendBroadcastAnnouncement();
                        toolStripStatusLabel1.Text = "Connected";
                        timer1.Stop();
                    }
                }
                else MessageBox.Show("No serial port available!");
            }
            else
            {
                toolStripStatusLabel1.Text = "Disconnecting...";
                com.Close();
                timer2.Stop();
                timer3.Stop();
                connect_b.Text = "Connect";
                port_cb.Enabled = true;
                toolStripStatusLabel1.Text = "Disconnected";
                timer1.Start();
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            portlist = SerialPort.GetPortNames();
        }

        private void com_DataReceived(object sender, EventArgs e)
        {
            byte[] buffer_au8;
            byte CRC_u8 = 0;
            int receivedBytes_i32 = com.BytesToRead;
            int bufferIndex_i32 = 0;
            if (0 < receivedBytes_i32)
            {
                buffer_au8 = new byte[receivedBytes_i32];
                while (0 < receivedBytes_i32)
                {
                    buffer_au8[bufferIndex_i32++] = (byte)com.BaseStream.ReadByte();
                    receivedBytes_i32--;
                }
                CRC_u8 = buffer_au8[buffer_au8.Length - 1];
                buffer_au8[buffer_au8.Length - 1] = 0;
                if (CRC_u8 == CalcChecksum(buffer_au8))
                {
                    HandleReceivedFrame(buffer_au8);
                }
                else
                {
                    richTextBox1.Text += "Frame with invalid CRC received\n";
                    foreach (byte db in buffer_au8)
                    {
                        richTextBox1.Text += db.ToString("X2") + " ";

                    }
                    richTextBox1.Text += "\n";
                }
            }
            

        }

        private void HandleReceivedFrame(byte [] frameBuffer_au8)
        {
            byte [] data_buffer_au8;
            
            {
                lock (richTextBox1.Text)
                {
                    if ((frameBuffer_au8[0] != myID_cu8))
                    {
                        /* From Radio */
                        if (frameBuffer_au8[0] == 0x68) richTextBox1.Text += "Frame from Radio ";
                        if (frameBuffer_au8[2u] == myID_cu8)
                        {
                            richTextBox1.Text += "to CDC ";
                            /* To CDC */
                            data_buffer_au8 = new byte[frameBuffer_au8[1u] - 2u]; // CRC and Dest dev
                            for (int i = 0; i < data_buffer_au8.Length; i++)
                            {
                                data_buffer_au8[i] = frameBuffer_au8[3 + i];
                            }
                            HandleCDC_Msg(data_buffer_au8);
                        }
                        richTextBox1.Text += "received: \n";
                    }
                }
                
            }
            
        }

        private void HandleCDC_Msg(byte [] data_buffer_au8)
        {
            if ((data_buffer_au8.Length == 1) && (data_buffer_au8[0] == 0x01))
            {
                /* Poll received */
                richTextBox1.Text += "with poll ";
                SendBroadcastPingResp();
            }
            else if ((data_buffer_au8.Length == 3) && (data_buffer_au8[0] == 0x38) && (data_buffer_au8[1] == 0x00) && (data_buffer_au8[2] == 0x00))
            {
                /* Request Current CD and Track Status */
                richTextBox1.Text += "with status req ";
                SendStatusFrame(globalStatus, CDEmu_currentDiskId_u8, CDEmu_currentTrackId_u8);
            }
            else if ((data_buffer_au8.Length == 3) && (data_buffer_au8[0] == 0x38) && (data_buffer_au8[1] == 0x01) && (data_buffer_au8[2] == 0x00))
            {
                /* Stop playing command */
                richTextBox1.Text += "with stop cmd ";
                SendStatusFrame(CDEmu_StatusType.STOPPED, CDEmu_currentDiskId_u8, CDEmu_currentTrackId_u8);
                globalStatus = CDEmu_StatusType.STOPPED;
                status_l.Text = "STOPPED";
            }
            else if ((data_buffer_au8.Length == 3) && (data_buffer_au8[0] == 0x38) && (data_buffer_au8[1] == 0x03) && (data_buffer_au8[2] == 0x00))
            {
                /* Start playing command */
                richTextBox1.Text += "with start cmd ";
                SendStatusFrame(CDEmu_StatusType.STARTING, CDEmu_currentDiskId_u8, CDEmu_currentTrackId_u8);
                globalStatus = CDEmu_StatusType.STARTED;
                status_l.Text = "STARTED";
            }
            else if ((data_buffer_au8.Length == 3) && (data_buffer_au8[0] == 0x38) && (data_buffer_au8[1] == 0x04) && (data_buffer_au8[2] == 0x00))
            {
                /* Fast forward command */
                richTextBox1.Text += "with ff cmd ";
                SendStatusFrame(CDEmu_StatusType.FASTFORWARD, CDEmu_currentDiskId_u8, CDEmu_currentTrackId_u8);
                globalStatus = CDEmu_StatusType.FASTFORWARD;
                status_l.Text = "FF";
            }
            else if ((data_buffer_au8.Length == 3) && (data_buffer_au8[0] == 0x38) && (data_buffer_au8[1] == 0x04) && (data_buffer_au8[2] == 0x01))
            {
                /* Fast backward command */
                richTextBox1.Text += "with fb cmd ";
                SendStatusFrame(CDEmu_StatusType.FASTBACKWARD, CDEmu_currentDiskId_u8, CDEmu_currentTrackId_u8);
                globalStatus = CDEmu_StatusType.FASTBACKWARD;
                status_l.Text = "FB";
            }
            else if ((data_buffer_au8.Length == 3) && (data_buffer_au8[0] == 0x38) && (data_buffer_au8[1] == 0x06))
            {
                /* Change disk to disk # command */
                CDEmu_currentTrackId_u8 = 1;
                CDEmu_currentDiskId_u8 = data_buffer_au8[2];
                diskSel.Value = CDEmu_currentDiskId_u8;
                trackSel.Value = CDEmu_currentTrackId_u8;
                richTextBox1.Text += "with ChangeDisk " + CDEmu_currentDiskId_u8 + " cmd ";
                SendStatusFrame(CDEmu_StatusType.STARTING, CDEmu_currentDiskId_u8, CDEmu_currentTrackId_u8);
                globalStatus = CDEmu_StatusType.STARTED;
                status_l.Text = "STARTED";
            }
            else if ((data_buffer_au8.Length == 3) && (data_buffer_au8[0] == 0x38) && (data_buffer_au8[1] == 0x07) && (data_buffer_au8[2] == 0x01))
            {
                /* Scan intro mode command */
                richTextBox1.Text += "with Intro ON cmd ";
                globalStatus = CDEmu_StatusType.STARTED;
                SendStatusFrame(globalStatus, CDEmu_currentDiskId_u8, CDEmu_currentTrackId_u8);
                status_l.Text = "STARTED";
            }
            else if ((data_buffer_au8.Length == 3) && (data_buffer_au8[0] == 0x38) && (data_buffer_au8[1] == 0x07) && (data_buffer_au8[2] == 0x00))
            {
                /* Scan intro mode command */
                richTextBox1.Text += "with Intro OFF cmd ";
                SendStatusFrame(globalStatus, CDEmu_currentDiskId_u8, CDEmu_currentTrackId_u8);
            }
            else if ((data_buffer_au8.Length == 3) && (data_buffer_au8[0] == 0x38) && (data_buffer_au8[1] == 0x08) && (data_buffer_au8[2] == 0x01))
            {
                /* Random mode command */
                richTextBox1.Text += "with RND ON cmd ";
                SendStatusFrame(globalStatus, CDEmu_currentDiskId_u8, CDEmu_currentTrackId_u8);
            }
            else if ((data_buffer_au8.Length == 3) && (data_buffer_au8[0] == 0x38) && (data_buffer_au8[1] == 0x08) && (data_buffer_au8[2] == 0x00))
            {
                /* Random mode command */
                richTextBox1.Text += "with RND OFF cmd ";
                SendStatusFrame(globalStatus, CDEmu_currentDiskId_u8, CDEmu_currentTrackId_u8);
            }
            else if ((data_buffer_au8.Length == 3) && (data_buffer_au8[0] == 0x38) && ((data_buffer_au8[1] == 0x0A) || (data_buffer_au8[1] == 0x05)) && (data_buffer_au8[2] == 0x00))
            {
                /* Next track command */
                CDEmu_currentTrackId_u8++;
                trackSel.Value = CDEmu_currentTrackId_u8;
                richTextBox1.Text += "with NXT cmd ";
                SendStatusFrame(globalStatus, CDEmu_currentDiskId_u8, CDEmu_currentTrackId_u8);
            }
            else if ((data_buffer_au8.Length == 3) && (data_buffer_au8[0] == 0x38) && ((data_buffer_au8[1] == 0x0A) || (data_buffer_au8[1] == 0x05)) && (data_buffer_au8[2] == 0x01))
            {
                /* Previous track command */
                if (1 < CDEmu_currentTrackId_u8 )
                {
                    CDEmu_currentTrackId_u8--;
                    trackSel.Value = CDEmu_currentTrackId_u8;
                }
                
                richTextBox1.Text += "with PRV cmd ";
                SendStatusFrame(globalStatus, CDEmu_currentDiskId_u8, CDEmu_currentTrackId_u8);
            }
            else
            {
                richTextBox1.Text += "with unknown cmd: ";
                foreach (byte dataByte in data_buffer_au8)
                {
                    richTextBox1.Text += dataByte.ToString("X2") + " ";
                }
            }
        }

        private void SendStatusFrame(CDEmu_StatusType status_en, byte diskId_u8, byte trackId_u8)
        {
            /* CD and Track Status Not Playing Response */
            /* 39 00 02 00 3F 00 dd tt */
            byte crc_u8 = 0;
            byte[] buffer_au8 = new byte[12]; /* Src dev + len + CRC + Dest dev + Data len*/
            switch (status_en)
            {
                case CDEmu_StatusType.STOPPED:
                    buffer_au8[0] = myID_cu8;
                    buffer_au8[1] = 2 + 8; /* CRC + Dest dev + Data len*/
                    buffer_au8[2] = 0x68;
                    buffer_au8[3] = 0x39;
                    buffer_au8[4] = 0x00;
                    buffer_au8[5] = 0x02; // Stopped
                    buffer_au8[6] = 0x00;
                    buffer_au8[7] = 0x3F;
                    buffer_au8[8] = 0x00;
                    buffer_au8[9] = diskId_u8;
                    buffer_au8[10] = (byte)(((trackId_u8 / 10) << 4) | (trackId_u8 % 10));
                    break;
                case CDEmu_StatusType.STARTING:
                    buffer_au8[0] = myID_cu8;
                    buffer_au8[1] = 2 + 8; /* CRC + Dest dev + Data len*/
                    buffer_au8[2] = 0x68;
                    buffer_au8[3] = 0x39;
                    buffer_au8[4] = 0x02; // Pending
                    buffer_au8[5] = 0x09; // Started
                    buffer_au8[6] = 0x00;
                    buffer_au8[7] = 0x3F;
                    buffer_au8[8] = 0x00;
                    buffer_au8[9] = diskId_u8;
                    buffer_au8[10] = (byte)(((trackId_u8 / 10) << 4) | (trackId_u8 % 10));
                    break;
                case CDEmu_StatusType.STARTED:
                    buffer_au8[0] = myID_cu8;
                    buffer_au8[1] = 2 + 8; /* CRC + Dest dev + Data len*/
                    buffer_au8[2] = 0x68;
                    buffer_au8[3] = 0x39;
                    buffer_au8[4] = 0x00;
                    buffer_au8[5] = 0x09; // Started
                    buffer_au8[6] = 0x00;
                    buffer_au8[7] = 0x3F;
                    buffer_au8[8] = 0x00;
                    buffer_au8[9] = diskId_u8;
                    buffer_au8[10] = (byte)(((trackId_u8 / 10) << 4) | (trackId_u8 % 10));
                    break;
                case CDEmu_StatusType.FASTFORWARD:
                    buffer_au8[0] = myID_cu8;
                    buffer_au8[1] = 2 + 8; /* CRC + Dest dev + Data len*/
                    buffer_au8[2] = 0x68;
                    buffer_au8[3] = 0x39;
                    buffer_au8[4] = 0x03; // FF
                    buffer_au8[5] = 0x09; // Started
                    buffer_au8[6] = 0x00;
                    buffer_au8[7] = 0x3F;
                    buffer_au8[8] = 0x00;
                    buffer_au8[9] = diskId_u8;
                    buffer_au8[10] = (byte)(((trackId_u8 / 10) << 4) | (trackId_u8 % 10));
                    break;
                case CDEmu_StatusType.FASTBACKWARD:
                    buffer_au8[0] = myID_cu8;
                    buffer_au8[1] = 2 + 8; /* CRC + Dest dev + Data len*/
                    buffer_au8[2] = 0x68;
                    buffer_au8[3] = 0x39;
                    buffer_au8[4] = 0x04; // FB
                    buffer_au8[5] = 0x09; // Started
                    buffer_au8[6] = 0x00;
                    buffer_au8[7] = 0x3F;
                    buffer_au8[8] = 0x00;
                    buffer_au8[9] = diskId_u8;
                    buffer_au8[10] = (byte)(((trackId_u8 / 10) << 4) | (trackId_u8 % 10));
                    break;
                case CDEmu_StatusType.STOPPING:
                    buffer_au8[0] = myID_cu8;
                    buffer_au8[1] = 2 + 8; /* CRC + Dest dev + Data len*/
                    buffer_au8[2] = 0x68;
                    buffer_au8[3] = 0x39;
                    buffer_au8[4] = 0x07; // Stopping
                    buffer_au8[5] = 0x09; // Started
                    buffer_au8[6] = 0x00;
                    buffer_au8[7] = 0x3F;
                    buffer_au8[8] = 0x00;
                    buffer_au8[9] = diskId_u8;
                    buffer_au8[10] = (byte)(((trackId_u8 / 10) << 4) | (trackId_u8 % 10));
                    break;
                case CDEmu_StatusType.SEEKING:
                    buffer_au8[0] = myID_cu8;
                    buffer_au8[1] = 2 + 8; /* CRC + Dest dev + Data len*/
                    buffer_au8[2] = 0x68;
                    buffer_au8[3] = 0x39;
                    buffer_au8[4] = 0x08; // Seeking
                    buffer_au8[5] = 0x09; // Started
                    buffer_au8[6] = 0x00;
                    buffer_au8[7] = 0x3F;
                    buffer_au8[8] = 0x00;
                    buffer_au8[9] = diskId_u8;
                    buffer_au8[10] = (byte)(((trackId_u8 / 10) << 4) | (trackId_u8 % 10));
                    break;
                default:
                    break;
            }
            
            crc_u8 = CalcChecksum(buffer_au8);
            buffer_au8[11] = crc_u8;
            //com.BaseStream.Write(buffer_au8, 0, 12);
        }

        private void SendBroadcastPingResp()
        {
            byte crc_u8 =0;
            byte[] buffer_au8 = new byte[6];
            buffer_au8[0] = myID_cu8;
            buffer_au8[1] = 4;
            buffer_au8[2] = 0xFF;
            buffer_au8[3] = 2;
            buffer_au8[4] = 0;
            crc_u8 = CalcChecksum(buffer_au8);
            buffer_au8[5] = crc_u8;
            //com.BaseStream.Write(buffer_au8, 0, 6);
        }

        private void SendBroadcastAnnouncement()
        {
            byte crc_u8 = 0;
            byte[] buffer_au8 = new byte[6];
            buffer_au8[0] = myID_cu8;
            buffer_au8[1] = 4;
            buffer_au8[2] = 0xFF;
            buffer_au8[3] = 2;
            buffer_au8[4] = 1;
            crc_u8 = CalcChecksum(buffer_au8);
            buffer_au8[5] = crc_u8;
            //com.BaseStream.Write(buffer_au8, 0, 6);
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            if (com.IsOpen)
            {

            }
            else
            {
                try
                {
                    com.Open();
                    toolStripStatusLabel1.Text = "Reconnected";
                }
                catch
                {
                    toolStripStatusLabel1.Text = "Connection Lost";
                    if (err_tmr_cnt == 50)
                    {
                        try
                        {
                            com.Close();
                        }
                        catch
                        {

                        }
                        connect_b.Enabled = true;
                        connect_b.Text = "Connect";
                        port_cb.Enabled = true;
                        err_tmr_cnt = 0;
                        timer1.Start();
                        timer2.Stop();

                    }
                    else
                    {
                        err_tmr_cnt++;
                    }
                }
            }
        }

        private void port_cb_DropDown(object sender, EventArgs e)
        {
            port_cb.DataSource = portlist;
        }

        private void connect_b_Click(object sender, EventArgs e)
        {
            serial_connect();
        }

        private void timer3_Tick(object sender, EventArgs e)
        {
            if (com.IsOpen && (false == pollReceived_b))
            {
                SendBroadcastAnnouncement();
            }
            else
            {
                timer3.Stop();
            }
        }

        private void trackSel_ValueChanged(object sender, EventArgs e)
        {
            CDEmu_currentTrackId_u8 = (byte)trackSel.Value;
            SendStatusFrame(globalStatus, CDEmu_currentDiskId_u8, CDEmu_currentTrackId_u8);
        }

        private void diskSel_ValueChanged(object sender, EventArgs e)
        {
            CDEmu_currentDiskId_u8 = (byte)diskSel.Value;
            SendStatusFrame(globalStatus, CDEmu_currentDiskId_u8, CDEmu_currentTrackId_u8);
        }
    }
}
