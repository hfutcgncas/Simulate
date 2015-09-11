using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


using System.Net;
using System.Net.Sockets;

using System.Threading;
using System.Windows;

using TableTennisV3.ToolResource;

namespace TableTennisV3.PresentationLayer.Models
{
    class cVision
    {
        Socket newsock;
        EndPoint Remote;
        int recv;

        public double[] hitPar = new double[13];

        byte[] data = new byte[5000];

        public void initRcv()
        {
            try
            {
                IPEndPoint ipep = new IPEndPoint(IPAddress.Any, Constants.Visionport);
                newsock = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                newsock.Bind(ipep);
                //----------------------------------------------------------------
                IPEndPoint sender = new IPEndPoint(IPAddress.Parse(Constants.VisionIP), Constants.Visionport);
                Remote = (EndPoint)sender;
            }
            catch(Exception e)
            {
                
            }
            
        }

        public void ReciveData()
        {
            if (Remote == null)
            {
                MessageBox.Show("Didn't conect");
            }
            else 
            { 
                recv = newsock.ReceiveFrom(data, ref Remote);
            }

            for (int i = 0; i < 13; i++)
            {
                hitPar[i] = BitConverter.ToDouble(data, i * 8);
            }
            return;
           // 
        }

       
    }
}
