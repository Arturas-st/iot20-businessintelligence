using IoTHubTrigger = Microsoft.Azure.WebJobs.EventHubTriggerAttribute;

using Microsoft.Azure.WebJobs;
using Microsoft.Azure.WebJobs.Host;
using Microsoft.Azure.EventHubs;
using System.Text;
using System.Net.Http;
using Microsoft.Extensions.Logging;
using Newtonsoft.Json;
using System;

namespace AzureFunctions
{
    public static class saveToCosmosDb
    {
        private static HttpClient client = new HttpClient();

        [FunctionName("saveToCosmosDb")]
        public static void Run(
            [IoTHubTrigger("messages/events", Connection = "iotHubConection", ConsumerGroup = "cosmosdb1")] EventData message,
            [CosmosDB(
                databaseName: "IOT20",
                collectionName: "Messages",
                CreateIfNotExists = true,
                ConnectionStringSetting = "cosmosDbConnection"
                
            )]out dynamic cosmos,
            ILogger log)
        
        {
            try
            {
                var msg = JsonConvert.DeserializeObject<DhtMeasurement>(Encoding.UTF8.GetString(message.Body.Array));
                msg.DeviceId = message.SystemProperties["iothub-connection-device-id"].ToString();
                msg.School = message.Properties["School"].ToString();
                msg.Name = message.Properties["Name"].ToString();
                
                
         

                var json = JsonConvert.SerializeObject(msg);
                cosmos = json;
                log.LogInformation($"Measurement was saved to Cosmos DB, Message::{json}");



            }
            catch(Exception e)
            {
            cosmos = null;
            log.LogInformation($"Unable to process Request, Error::{e.Message}");


            }

            
           
            

        }
    }
}