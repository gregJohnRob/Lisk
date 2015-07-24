using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.ServiceModel;
using System.ServiceModel.Web;
using System.ServiceModel.Description;

using E.Lisk.Api.Entities;
using E.Lisk.Api.Services;
using E.Lisk.Api.Services.Impl;

namespace E.Lisk.Api.Host
{
    public class Program
    {
        static void Main(string[] args)
        {
            WebHttpBinding binding = new WebHttpBinding();
            WebHttpBehavior behavior = new WebHttpBehavior();

            GameService aGameService = new GameService();
            LobbyService aLobbyService = new LobbyService();

            WebServiceHost aGameHost = new WebServiceHost(aGameService, new Uri("http://localhost:22415/Lisk/v1/"));
            WebServiceHost aLobbyHost = new WebServiceHost(aLobbyService, new Uri("http://localhost:22415/Lisk/v1/"));
            try
            {
                aGameHost.AddServiceEndpoint(typeof(IGameService), binding, "game");
                aLobbyHost.AddServiceEndpoint(typeof(ILobbyService), binding, "lobby");
                aGameHost.Open();
                aLobbyHost.Open();

                Console.WriteLine(string.Format("Starting GameService {0}\nStarting LobbyService {1}",
                                        ServiceVersion.GameServiceVersion.Version,
                                        ServiceVersion.LobbyServiceVersion.Version));

                Console.WriteLine("Press any key to close...");
            }
            catch (Exception e)
            {
                Console.WriteLine("Error starting Endpoint...");
                Console.WriteLine(e.Message);
            }
            Console.ReadKey();
            Console.WriteLine("Closing...");
            aGameHost.Close();
        }
    }
}
