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
using System.Threading;
using System.Diagnostics;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form, IObserver<FrameStatus>
    {
        const int w = 320;
        const int h = 240;
        const int QVGA = w * h * 2;


        private SerialFrameDAQ DAQ;

        public Form1()
        {
            InitializeComponent();

        
            this.progressBar1.Maximum = 100;
            this.progressBar1.Minimum = 0;
            this.progressBar1.Step = 1;

            DAQ = new SerialFrameDAQ(DAQFinished);
            DAQ.Subscribe(this);

            comboBox1.Items.AddRange(DAQ.GetAvailablePorts());
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (comboBox1.SelectedItem != null)
            {
                bool bStatus = DAQ.Open(comboBox1.SelectedItem as string);

                if(bStatus)
                {
                    this.button1.Enabled = false;
                    this.progressBar1.Value = 0;

                    DAQ.StartCapture(w, h, 2);
                }
                else
                {
                    MessageBox.Show("Cannot open COM Port.");
                }
            }
        }
        
        private void DAQFinished(byte[] buffer)
        {
            DAQ.Close();
            GetImage(buffer);

            this.Invoke(
                new Action(
                    delegate
                    {
                        this.button1.Enabled = true; 
                        this.Update();
                    }
                )
            );
  
        }

        private void GetImage(byte[] buffer)
        {
            Bitmap img = null;

            if ((buffer != null) && (buffer.Length == QVGA))
            {
                img = new Bitmap(w, h);

                int i = 0;

                for (int y = 0; y < h; y++)
                {
                    for (int x = 0; x < w; x++)
                    {
                        byte d1 = buffer[i];
                        byte d2 = buffer[i + 1];

                        byte r1 = (byte)((d1 & 0xF8) << 0);
                        byte g1 = (byte)(((d1 & 0x07) << 5) | ((d2 & 0xE0) >> 3));
                        byte b1 = (byte)((d2 & 0x1F) << 3);

                        img.SetPixel(x, y, Color.FromArgb(r1, g1, b1));

                        i += 2;
                    }
                }
            }

            this.Invoke(
                new Action(
                    delegate
                    {
                        this.pictureBox1.Image = img;
                        this.pictureBox1.Refresh();
                        this.Update();
                    }
                )
            );
        }

        public void OnNext(FrameStatus value)
        {
            this.Invoke(
                new Action(
                    delegate
                    {
                        this.progressBar1.Value = (int)value.Status;
                        this.Update();
                        Debug.WriteLine(value.Status);
                    }
                )
            );
        }

        public void OnCompleted()
        {
            this.Invoke(
                new Action(
                    delegate
                    {
                        this.progressBar1.Value = 0;
                        this.Update();
                        
                    }
                )
            );
        }


        public void OnError(Exception error)
        {
            throw new NotImplementedException();
        }
    }

    public class FrameStatus
    {
        public float Status { get; private set; }

        public FrameStatus(float status)
        {
            this.Status = status;
        }
    }

    abstract class FrameDAQ : IObservable<FrameStatus>
    {
        public delegate void DAQ_Finished(byte[] array);

        protected DAQ_Finished Callback { get; set; }
        private byte[] buffer;
        private int index;
        private List<IObserver<FrameStatus>> observers;

        public FrameDAQ(DAQ_Finished callback)
        {
            this.observers = new List<IObserver<FrameStatus>>();
            this.Callback = callback;
        }

        public void StartCapture(int w, int h, int SizeOfPixel)
        {
            buffer = new byte[w * h * SizeOfPixel];
            index = 0;

            StartDAQ();
        }

        protected void AppendBytes(byte[] array)
        {
            int bytesToRead = array.Length;

            if (this.index < this.buffer.Length)
            {
                if ((this.index + bytesToRead) <= this.buffer.Length)
                {
                    Array.Copy(array, 0, this.buffer, this.index, bytesToRead);
                    this.index += bytesToRead;

                    if (this.index >= this.buffer.Length)
                    {
                        foreach (var observer in observers)
                        {
                            observer.OnCompleted();
                        }

                        this.Callback.Invoke(this.buffer);
                    }
                    else
                    {
                        foreach (var observer in observers)
                        {
                            observer.OnNext(new FrameStatus(100 * (float)this.index / this.buffer.Length));
                        }
                    }
                }
            }
        }

        protected abstract void StartDAQ();

        public IDisposable Subscribe(IObserver<FrameStatus> observer)
        {
            if (!observers.Contains(observer))
                observers.Add(observer);

            return new Unsubscriber(observers, observer);
        }

        private class Unsubscriber : IDisposable
        {
            private List<IObserver<FrameStatus>> _observers;
            private IObserver<FrameStatus> _observer;

            public Unsubscriber(List<IObserver<FrameStatus>> observers, IObserver<FrameStatus> observer)
            {
                this._observers = observers;
                this._observer = observer;
            }

            public void Dispose()
            {
                if (_observer != null && _observers.Contains(_observer))
                    _observers.Remove(_observer);
            }
        }
    }

    class SerialFrameDAQ : FrameDAQ
    {
        private SerialPort Serial;

        public SerialFrameDAQ(DAQ_Finished callback)
            : base(callback)
        {

        }

        public string[] GetAvailablePorts()
        {
            return SerialPort.GetPortNames();
        }

        public bool Open(string port)
        {
            Serial = new SerialPort(port, 115200, Parity.None, 8, StopBits.One);

            try
            {
                Serial.Open();
                Serial.DataReceived += Serial_DataReceived;

                return Serial.IsOpen;
            }
            catch
            {

            }

            return false;
        }

        public void Close()
        {
            if (Serial != null && Serial.IsOpen)
            {
                try
                {
                    Serial.Close();
                }
                catch
                {

                }
            }

            Serial.Dispose();
            Serial = null;
        }

        protected override void StartDAQ()
        {
            if (Serial.IsOpen)
            {
                Serial.Write(new byte[] { (byte)'e' }, 0, 1);
            }
        }

        private void Serial_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int bytesToread = Serial.BytesToRead;
            byte[] buffer = new byte[bytesToread];
            Serial.Read(buffer, 0, bytesToread);

            base.AppendBytes(buffer);
        }
    }
}
