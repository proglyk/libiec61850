#include "net_if.h"

// Определение типов

// Тип упр. структуры
typedef struct {
  // локальные
  s32_t null_var;
} ctx_t;

// Объявления функций

// Создание, инициализация и удаление экземпляра упр. структуры
static void *conn_init(sess_init_cb_ptr_t, net_if_data_t *, void*);
static signed long conn_do(void*);
static void  conn_del(sess_del_cb_ptr_t, void*);

// Переменные, константы

// Реализация интерфейса net_if
const net_if_fn_t xFnSrvUpvs = {
  .pvUpperInit =   (upper_init_ptr_t)NULL/* upvs_srv_init */,
  .pvUpper =       (void *)NULL/* &xBroker */,
  .ppvSessInit =   (sess_init_ptr_t)conn_init,
  .ppvSessInitCb = (sess_init_cb_ptr_t)NULL,
  .pslSessDo =     (sess_do_ptr_t)conn_do,
  .pvSessDel =     (sess_del_ptr_t)conn_del,
  .pvSessDelCb =   (sess_del_cb_ptr_t)NULL,
};

/**	----------------------------------------------------------------------------
	* @brief Connection init */
static void *
  conn_init(sess_init_cb_ptr_t pFn, net_if_data_t *pdata, void* pvPld) {
/*----------------------------------------------------------------------------*/
  ctx_t *ctx;
  if (!pdata) return NULL;
  
  ctx = malloc(sizeof(ctx_t));
  if (!ctx) return NULL;
  
/*   pxeth->Ied.pxServer->isoServer = IsoServer_create();
  // настройка
	IsoServer_setConnectionHandler(pxeth->Ied.pxServer->isoServer, 
		isoConnectionIndicationHandler, (void*) pxeth->Ied.pxServer->mmsServer);
	pxeth->Ied.pxServer->isoServer->tcpPort = pxeth->Ied.ulPort;
	pxeth->Ied.pxServer->isoServer->tcpIp = pxeth->xNetif.ip_addr.addr;
  // 1. запустить поток ied
	xTaskCreate((void (*)(void*))IsoServer_Listen2, "Ied", (15*configMINIMAL_STACK_SIZE), 
		pxeth->Ied.pxServer->isoServer, makeFreeRtosPriority(osPriorityIdle), 
		&(pxeth->Ied.pvHandle));
  pxeth->Ied.usState = SVR_RUNNING; */

  return (void *)ctx;
}

/**	----------------------------------------------------------------------------
	* @brief 
	* @param sock: Сокет подключения.
	* @param ptr: Указатель на упр.структуру. //TODO Надо новый 
	* @retval error: Статус выполнения. */
static signed long
  conn_do(void* argv) {
/*----------------------------------------------------------------------------*/
  s32_t rc;
  ctx_t *ctx = (ctx_t *)argv;
  
  // проверка арг-тов
  if (!ctx) return -1;
  
  // ...
  
	//vTaskDelay(250);
	return 0;
  
  exit:
  // выход в случае потери связи
  return -1;
}

/**	----------------------------------------------------------------------------
	* @brief ??? */
static void
  conn_del(sess_del_cb_ptr_t pFn, void *argv) {
/*----------------------------------------------------------------------------*/
  ctx_t* ctx = (ctx_t *)argv;
  
  if (!ctx) return;
  
  // выполнить обратную связь
  if (pFn) pFn();
  
  free(ctx);
}
