using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.ServiceModel;
using System.ServiceModel.Activation;
using System.ServiceModel.Web;

using E.Lisk.Api.Entities;

namespace E.Lisk.Api.Services.Impl
{
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.Single,
                     ConcurrencyMode = ConcurrencyMode.Single,
                     IncludeExceptionDetailInFaults = true)]
    [AspNetCompatibilityRequirements(RequirementsMode = AspNetCompatibilityRequirementsMode.Allowed)]
    public class LobbyService : ILobbyService
    {
        public ServiceVersion Version()
        {
            return ServiceVersion.LobbyServiceVersion;
        }
    }
}
