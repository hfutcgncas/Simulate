using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TableTennisV3.ToolResource
{
    static class Constants
    {
        //------------------------------------
        private const double LEAD_X = 160.0;// 导程 tooth/mm
        private const double LEAD_Y = 220.5;
        private const double LEAD_Z = 163.3;

        private const int tooth_NO = 30000;//tooth
        //------------------------------------
        public const double unit_X =  tooth_NO / LEAD_X; //187.5   counts/mm
        public const double unit_Y = tooth_NO / LEAD_Y; //136.05  counts/mm

        //------------------------------------
        public const double MAX_X =  550; // mm
        public const double MIN_X = -550;
        public const double MAX_Y =  250;
        public const double MIN_Y = -100;

        public const double MAX_VX = 2; // m/s
        public const double MIN_VX = 0;
        public const double MAX_VY = 2; 
        public const double MIN_VY = 0;

        public const int SMax = 165;
        public const int SMin = 80;
        public const int PMax = 160;
        public const int PMin = 110;
        public const int ZMax = 430;
        public const int ZMin = 240;


        public const double V_factor = 10; 
        //------------------------------------
        //=================================================================================================
        //------------------------------------
        public const int Visionport = 7000;
        //public const string VisionIP = "192.168.64.5";
        public const string VisionIP = "127.0.0.1";
        //------------------------------------
        //=================================================================================================
        //------------------------------------
        public const int PMAC_Progm_NO = 9;
    }
}
