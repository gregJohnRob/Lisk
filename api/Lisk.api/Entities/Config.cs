using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace E.Lisk.Api.Entities
{
    internal class Config
    {
    }

    public interface IServiceVersion
    {
        string Version { get; set; }
    }

    public class ServiceVersion : IServiceVersion
    {
        public const string lGameServiceVersion = "0.0.0.0(A)";
        public const string lLobbyServiceVersion = "0.0.0.0(A)";

        public string Endpoint { get; set; }
        public string Version { get; set; }

        public static ServiceVersion GameServiceVersion  { get { return new ServiceVersion() { Version = lGameServiceVersion, Endpoint = "endpoint://game"  }; } }
        public static ServiceVersion LobbyServiceVersion { get { return new ServiceVersion() { Version = lLobbyServiceVersion, Endpoint = "endpoint://lobby" }; } }
    }
}
