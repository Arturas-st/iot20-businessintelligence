using System;
using System.Collections.Generic;
using System.Text;

namespace AzureFunctions
{
    public class DhtMeasurement
    {
        public string DeviceId { get; set; }
        public string deviceType { get; set; }
        public float temp { get; set; }
        public float hum { get; set; }
        public long ts { get; set; }
        public string Name { get; set; }
        public string School { get; set; }
              
    }
}
