

static int internet_config( void );

// Внешний код - Сервер UPVS
extern net_if_fn_t xFnSrvIec61850;
static IsoServerPld_t xPldMms;
// static IsoServerPld_t xPldMms = {
  // .pvMmsHandler = isoConnectionIndicationHandler,
  // .pvMmsPayload = NULL
// };

int main(void) {

  
	for (;;) { }
}

static int internet_config(void) {
  s32_t rc;
	net_init_t init_str;

	//
	rc = net__init(net__inst());
  if (rc<0) {
    LWIP_DEBUGF( NET_DEBUG, ("ETH couldn't be started, in '%s' /NET/net_netif.c:%d\r\n", 
      __FUNCTION__, __LINE__) );
    goto exit;
  }
  
  //
  xPldMms.pvMmsHandler = isoConnectionIndicationHandler;
  xPldMms.pvMmsPayload = NULL;
	//
	init_str.pcName = "iec61850_srv";
	init_str.pxFn = &xFnSrvIec61850;
  init_str.bEnabled = true;
	init_str.ulPort = 102;
  init_str.pvTopPld = (void *)&xPldMms;
	rc = net__add_srv(net__inst(), &init_str);
  if (rc < 0) {
    LWIP_DEBUGF( NET_DEBUG, ("\"%s\" hasn't added...Err=%01d, in '%s' /main.c:%d\r\n", 
      init_str.pcName, rc, __FUNCTION__, __LINE__) );
    goto exit;
  }
  
	// запуск
	net__run(net__inst());
  return 0;
  
exit:
  return -1;
}
