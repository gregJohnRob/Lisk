using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.ServiceModel;
using System.ServiceModel.Web;

using E.Lisk.Api.Entities;

namespace E.Lisk.Api.Services
{
    [ServiceContract(Name = "GameService")]
    public interface IGameService
    {
        [OperationContract]
        [WebGet(UriTemplate = "/version", BodyStyle = WebMessageBodyStyle.Bare, ResponseFormat=WebMessageFormat.Json)]
        ServiceVersion Version();
    }
}
