using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.ComponentModel;
using System.Windows.Input;
using System.Windows;
using System.Collections.ObjectModel;

using TableTennisV3.PresentationLayer.Models;
using TableTennisV3.MVVM.Common;

using System.Diagnostics;
using System.Threading;

using TableTennisV3.ToolResource;


namespace TableTennisV3.PresentationLayer.ViewModels
{
    class BatViewModel : INotifyPropertyChanged
    {
        //INotifyPropertyChanged Members 不需更改---------------------------------------------------------
        #region INotifyPropertyChanged Members

        /// <summary>
        /// Event to which the view's controls will subscribe.
        /// This will enable them to refresh themselves when the binded property changes provided you fire this event.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// When property is changed call this method to fire the PropertyChanged Event
        /// </summary>
        /// <param name="propertyName"></param>
        public void OnPropertyChanged(string propertyName)
        {
            //Fire the PropertyChanged event in case somebody subscribed to it
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion

        //Private Variables 包括变量和命令的定义---------------------------------------------------------
        #region Private Variables
        /*The Variables are meant to be readonly as we mustnot change the address of any of them by creating new instances.
         *Problem with new istances is that since address changes the binding becomes invalid.
         *Instantiate all the variables in the constructor.
         */
        private readonly cBat bat; //控制球拍
        private readonly cVision vision; //接收视觉结果

        private readonly ICommand _HomeCmd;
        private readonly ICommand _HitBallCmd;
        private readonly ICommand _MoveToCmd;

        private readonly ICommand _ConectToPMACCmd;
        private readonly ICommand _SendToPMACCmd;
        private readonly ICommand _PMACServoONCmd;
        private readonly ICommand _PMACRunPgmCmd;
        private readonly ICommand _PMACServoOFFCmd;

        private readonly ICommand _ConectToZSPCmd;


        private readonly ICommand _ConectToVisionCmd;
       // private readonly ICommand _SendToZSPCmd;

        private Process ZSPClient_exe;
        #endregion

        //构造函数 完成变量的初始化和命令函数的绑定---------------------------------------------------------
        #region Constructors

        /// <summary>
        /// Instatiates all the readonly variables
        /// </summary>
        public BatViewModel()
        {
            bat = new cBat();
            vision = new cVision();

            //------------------------------
            _HomeCmd    = new RelayCommand(Home, CanHome);
            _HitBallCmd = new RelayCommand(HitBall, CanHitBall);
            _MoveToCmd  = new RelayCommand(MoveTo, CanMoveTo);

            _ConectToPMACCmd    = new RelayCommand(ConectToPMAC, CanConectToPMAC);
            _SendToPMACCmd      = new RelayCommand(SendToPMAC, CanSendToPMAC);
            _PMACServoONCmd     = new RelayCommand(PMACServoON, CanPMACServoON);
            _PMACRunPgmCmd      = new RelayCommand(PMACRunPgm, CanPMACRunPgm);
            _PMACServoOFFCmd    = new RelayCommand(PMACServoOFF, CanPMACServoOFF);

            _ConectToZSPCmd = new RelayCommand(ConectToZSP, CanConectToZSP);

            _ConectToVisionCmd = new RelayCommand(ConectToVision, CanConectToVision);
            //------------------------------
            ZSPClient_exe = new Process();

        }

        #endregion

        //Properties & Commands 用于交互的变量和命令---------------------------------------------------------
        #region Properties & Commands

        #region Commands


        public ICommand ConectToPMACCmd { get { return _ConectToPMACCmd; } }

        public ICommand SendToPMACCmd { get { return _SendToPMACCmd; } }

        public ICommand MoveToCmd { get { return _MoveToCmd; } }

        public ICommand ConectToZSPCmd { get { return _ConectToZSPCmd; } }

        public ICommand PMACServoONCmd { get { return _PMACServoONCmd; } }

        public ICommand PMACRunPgmCmd { get { return _PMACRunPgmCmd; } }

        public ICommand PMACServoOFFCmd { get { return _PMACServoOFFCmd; } }


        public ICommand ConectToVisionCmd { get { return _ConectToVisionCmd; } }
        #endregion

        #region Properties
        public int MAX_X = (int)Constants.MAX_X;
        public int MIN_X = (int)Constants.MIN_X;
        public int MAX_Y = (int)Constants.MAX_Y;
        public int MIN_Y = (int)Constants.MIN_Y;


        public double Hit_X
        {
            get { return bat.Hit_X; }
            set
            {
                bat.Hit_X = value;
                OnPropertyChanged("Hit_X");
            }
        }

        public double Hit_VX
        {
            get { return bat.Hit_VX; }
            set
            {
                bat.Hit_VX = value;
                OnPropertyChanged("Hit_VX");
            }
        }


        public double Hit_Y
        {
            get { return bat.Hit_Y; }
            set
            {
                bat.Hit_Y = value;
                OnPropertyChanged("Hit_Y");
            }
        }

        public double Hit_VY
        {
            get { return bat.Hit_VY; }
            set
            {
                bat.Hit_VY = value;
                OnPropertyChanged("Hit_VY");
            }
        }


        public int Hit_Z
        {
            get { return bat.Hit_Z; }
            set
            {
                bat.Hit_Z = value;
                OnPropertyChanged("Hit_Z");
            }
        }

        public int Hit_S
        {
            get { return bat.Hit_S; }
            set
            {
                bat.Hit_S = value;
                OnPropertyChanged("Hit_S");
            }
        }

        public int Hit_P
        {
            get { return bat.Hit_P; }
            set
            {
                bat.Hit_P = value;
                OnPropertyChanged("Hit_P");
            }
        }

        public string PMAC_msg
        {
            get { return bat.pmac_msg; }
            set
            {
                OnPropertyChanged("PMAC_msg");
            }
        }
        public string PMAC_cmd
        {
            get { return bat.pmac_card.m_PMAC_cmd; }
            set
            {
                bat.pmac_card.m_PMAC_cmd = value;
                OnPropertyChanged("PMAC_cmd");
            }
        }

        private string vision_res = "";
        public string VisionResult
        {
            get { return vision_res; } 
            set
            {
                vision_res = value;
                OnPropertyChanged("VisionResult");
            }
        }

        public int programNO;
        #endregion
        #endregion

        //Commands 命令函数的具体定义---------------------------------------------------------
        #region Commands

        #region Motion

        public bool CanHome(object obj)
        {
            //Enable the Button in some situation
            return true;
        }
        public void Home(object obj)
        {
            if (bat.Home())
                return;
        }
        //----------------------------------------------------------
        public bool CanHitBall(object obj)
        {
            //Enable the Button in some situation
            return true;
        }
        public void HitBall(object obj)
        {
            if (bat.HitBall())
                return;
        }
        //----------------------------------------------------------
        public bool CanMoveTo(object obj)
        {
            //Enable the Button in some situation
            return true;
        }
        public void MoveTo(object obj)
        {
            if (bat.MoveTo())
                return;
        }
        //----------------------------------------------------------
        public bool CanConectToPMAC(object obj)
        {
            //Enable the Button in some situation
            return true;
        }
        public void ConectToPMAC(object obj)
        {
           
            bat.pmac_card.ConectToPMAC();
            PMAC_msg = bat.pmac_msg;
        }
        //----------------------------------------------------------
        public bool CanSendToPMAC(object obj)
        {
            //Enable the Button in some situation
            return true;
        }
        public void SendToPMAC(object obj)
        {
            bat.pmac_card.m_PMAC_cmd = PMAC_cmd;
            bat.pmac_card.SendCMD();
            PMAC_msg = bat.pmac_msg;
            PMAC_cmd = "";
        }
        //----------------------------------------------------------
        public bool  CanConectToZSP(object obj)
        {
            //Enable the Button in some situation
            return true;
        }
        public void ConectToZSP(object obj)
        {
            bat.ZSP_Sender.StartWaitting();
            Thread.Sleep(100);
            
            try
            {
                ZSPClient_exe.StartInfo.FileName = "ZSPClient.exe";
                ZSPClient_exe.StartInfo.WindowStyle = ProcessWindowStyle.Minimized;
                ZSPClient_exe.Start();
            }
            catch(Exception e)
            {
                MessageBox.Show(e.Message);
            }

        }
        //---------------------------------------------------------- 开运动控制卡伺服
        public bool CanPMACServoON(object obj)
        {
            return true;
        }
        public void PMACServoON(object obj)
        {
            bat.pmac_card.ServoON();
            PMAC_msg = bat.pmac_msg;
        }
        //---------------------------------------------------------- 关运动控制卡伺服
        public bool CanPMACServoOFF(object obj)
        {
            return true;
        }
        public void PMACServoOFF(object obj)
        {
            bat.pmac_card.ServoOFF();
            PMAC_msg = bat.pmac_msg;
        }
        
        //---------------------------------------------------------- 运行程序
        public bool CanPMACRunPgm(object obj)
        {
            return true;
        }
        public void PMACRunPgm(object obj)
        {
            bat.pmac_card.RunProgram(Constants.PMAC_Progm_NO);
        }
        #endregion //motion

        #region Vision
        //----------------------------------------------------------
        public bool CanConectToVision(object obj)
        {
            //Enable the Button in some situation
            return true;
        }
        public void ConectToVision(object obj)
        {

            vision.initRcv();
            Thread RcvThread = new Thread(new ThreadStart(delegate
            {
                while (true)
                {
                    vision.ReciveData();
                    VisionResult = "   X  = " + vision.hitPar[1].ToString() +
                                   "\n Y  = " + vision.hitPar[2].ToString() +
                                   "\n Z  = " + vision.hitPar[3].ToString() +
                                   "\n Vx = " + vision.hitPar[4].ToString() +
                                   "\n Vy = " + vision.hitPar[5].ToString() +
                                   "\n Vz = " + vision.hitPar[6].ToString() +
                                   "\n tg = " + vision.hitPar[12].ToString();
                    Hit_X = vision.hitPar[1];
                    Hit_Y = 200;
                   // bat.pmac_card.m_PMAC_cmd = "P4 = 1";
                   // bat.pmac_card.SendCMD();

                    // MessageBox.Show("get here");
                    Thread.Sleep(50);
                }
            }
            ));
            RcvThread.Start();

        }
        //----------------------------------------------------------
        #endregion //Vision
        #endregion
    }
}
