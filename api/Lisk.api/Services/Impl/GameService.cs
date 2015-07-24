using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.ServiceModel;
using System.ServiceModel.Web;
using System.ServiceModel.Activation;

using E.Lisk.Api.Entities;

namespace E.Lisk.Api.Services.Impl
{
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.Single,
                     ConcurrencyMode = ConcurrencyMode.Single, 
                     IncludeExceptionDetailInFaults = true)]
    [AspNetCompatibilityRequirements(RequirementsMode = AspNetCompatibilityRequirementsMode.Allowed)]
    public class GameService : IGameService
    {
        public ServiceVersion Version()
        {
            return ServiceVersion.GameServiceVersion;
        }
    }
}
