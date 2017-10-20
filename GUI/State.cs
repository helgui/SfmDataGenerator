using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GUI
{
    class State
    {
        public State()
        {
            Suffix = "";
            CurrentDataset = "";    
        }
        public string DatasetType
        {
            get
            {
                if (Suffix == "")
                    return "sfm";
                if (Suffix == "s")
                    return "silhouette";
                if (Suffix == "d")
                    return "rgb-d";
                return "";
            }
        }
        public string Suffix;
        public string CurrentDataset;
    }
}
