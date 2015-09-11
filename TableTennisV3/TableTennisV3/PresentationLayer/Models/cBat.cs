
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;



using TableTennisV3.ToolResource;

namespace TableTennisV3.PresentationLayer.Models
{
    class cBat  
    {
        //public const int SMax = 165;
        //public const int SMin = 80;
        //public const int PMax = 160;
        //public const int PMin = 110;
        //public const int ZMax = 430;
        //public const int ZMin = 240;


        public cBat()
        {
            Hit_X = 0;
            hit_vx = 0.1;
            Hit_Y = 0;
            hit_vy = 0.1;

            Hit_Z = 300;
            Hit_S = 127;
            Hit_P = 127;

            pmac_card  = new cPMAC();
            ZSP_Sender = new cTCPListenor();

            
        }
        #region Variable
        #region Hit Variables
        //--------------
        private double hit_x;
        public double Hit_X
        {
            get { return hit_x; }
            set
            {
                if (value > Constants.MAX_X) value = Constants.MAX_X;
                if (value < Constants.MIN_X) value = Constants.MIN_X;
                hit_x = value;
                if (pmac_card != null && pmac_card.m_bDeviceOpen)
                {
                    Set_PMAC_P("p1", hit_x * Constants.unit_X );
                }
            }
        }

        private double hit_vx;
        public double Hit_VX //
        {
            get { return hit_vx; }
            set
            {
                if (value > Constants.MAX_VX) value = Constants.MAX_VX;
                if (value < Constants.MIN_VX) value = Constants.MIN_VX;
                hit_vx = value;
                if (pmac_card != null && pmac_card.m_bDeviceOpen)
                {
                    Set_PMAC_P("p9", hit_vx * Constants.unit_X * Constants.V_factor); //转化为cnt/ms
                }
            }
        }

        //--------------
        private double hit_y;
        public double Hit_Y
        {
            get { return hit_y; }
            set
            {
                if (value > Constants.MAX_Y) value = Constants.MAX_Y;
                if (value < Constants.MIN_Y) value = Constants.MIN_Y;
                hit_y = value;
                if (pmac_card != null && pmac_card.m_bDeviceOpen)
                {
                    Set_PMAC_P("p2", (int)(hit_y * Constants.unit_Y));
                }
            }
        }

        private double hit_vy;
        public double Hit_VY
        {
            get { return hit_vy; }
            set
            {
                if (value > Constants.MAX_VY) value = Constants.MAX_VY;
                if (value < Constants.MIN_VY) value = Constants.MIN_VY;
                hit_vy = value;
                if (pmac_card != null && pmac_card.m_bDeviceOpen)
                {
                    Set_PMAC_P("p10", hit_vy * Constants.unit_Y * Constants.V_factor); //转化为cnt/ms
                }
            }
        }

        //-------------- 
        private int hit_z;
        public int Hit_Z
        {
            get { return hit_z; }
            set
            {
                if (value > Constants.ZMax) value = Constants.ZMax;
                if (value < Constants.ZMin) value = Constants.ZMin;
                hit_z = value;
                if (ZSP_Sender != null && ZSP_Sender.isConected)
                {
                    ZSP_Sender.SendMsg("z" + (hit_z - Constants.ZMin).ToString() + "s" + hit_s.ToString() + "p" + hit_p.ToString() + "E");//这里- Constants.ZMin是为了满足UCHAR的类型范围。
                }
            }
        }

        private int hit_s;
        public int Hit_S
        {
            get { return hit_s; }
            set
            {
                if (value > Constants.SMax) value = Constants.SMax;
                if (value < Constants.SMin) value = Constants.SMin;
                hit_s = value;
                if (ZSP_Sender != null && ZSP_Sender.isConected)
                {
                    ZSP_Sender.SendMsg("z" + hit_z.ToString() + "s" + hit_s.ToString() + "p" + hit_p.ToString() + "E");
                }
            }
        }

        private int hit_p;
        public int Hit_P
        {
            get { return hit_p; }
            set
            {
                if (value > Constants.PMax) value = Constants.PMax;
                if (value < Constants.PMin) value = Constants.PMin;
                hit_p = value;
                if (ZSP_Sender != null && ZSP_Sender.isConected)
                {
                    ZSP_Sender.SendMsg("z" + hit_z.ToString() + "s" + hit_s.ToString() + "p" + hit_p.ToString() + "E");
                   // ZSP_Sender.WaitForReceive();
                }
            }
        }
        #endregion

        #region Realtime Variables
        public double Realtime_X { get; set; }
        public double Realtime_Y { get; set; }
        public double Realtime_Z { get; set; }
        public double Realtime_S { get; set; }
        public double Realtime_P { get; set; }
        #endregion

        #region PMAC Variables
        public cPMAC pmac_card;
        public string pmac_msg { get { return pmac_card.m_PMAC_msg; } }
        #endregion

        public cTCPListenor ZSP_Sender;

        #endregion
        //    private cUDPServer ZSP_Interface;
        //----------------------------------------
        //----------------------------------------
        #region Method
        public bool Home()
        {
            return false;
        }

        public bool MoveTo()
        {
      //      ZSP_Interface.ReciveData();
            byte[] ZSP_Target = new byte[4];
            ZSP_Target[0] = (byte)Hit_Z;
            ZSP_Target[1] = (byte)Hit_S;
            ZSP_Target[2] = (byte)Hit_P;

       //     ZSP_Interface.SendData(ZSP_Target);
            return false;
        }

        public bool HitBall()
        {
            return false;
        }

        public bool Set_PMAC_P(string px, double  value)
        {
            pmac_card.m_PMAC_cmd = px + "=" + value.ToString();
            pmac_card.SendCMD();
            return false;
        }

        #endregion
    }
}
