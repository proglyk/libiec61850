#include "libiec61850/IsoPresentation.h"
#include "lwip/opt.h"
#include "iso8823/CPAPPDU.h"


static void
	writer(asn_TYPE_descriptor_t * pxType, CPAPPDU_t* pxCpappdu, SBuffer * sbuf);


IsoPresStatus 
	codeSCPAMessage(SBuffer* sbuf) {
  
	CPAPPDU_t* cpatype;

  cpatype = calloc(1, sizeof(CPAPPDU_t));

  cpatype->modeSelector.modeValue = modeValue_normalmode;

  cpatype->normalModeParameters = calloc(1, sizeof(struct normalModeParameters));

  OCTET_STRING_t* respondingSelector = calloc(1, sizeof(OCTET_STRING_t));

  respondingSelector->size = 4;
  respondingSelector->buf = calloc(1, 4);
  respondingSelector->buf[0] = 0;
  respondingSelector->buf[1] = 0;
  respondingSelector->buf[2] = 0;
  respondingSelector->buf[3] = 1;

  ContextDefResList_t* pdvResultList = calloc(1, sizeof(ContextDefResList_t));

  pdvResultList->list.count = 2;

  pdvResultList->list.array = calloc(2, sizeof(ContextDefResListSeq_t*));

  pdvResultList->list.array[0] = calloc(1, sizeof(ContextDefResListSeq_t));

  pdvResultList->list.array[0]->result = result_acceptance;

  pdvResultList->list.array[0]->providerreason = 0;

  OBJECT_IDENTIFIER_t* transfersyntax = calloc(1, sizeof(OBJECT_IDENTIFIER_t));

  transfersyntax->size = 2;
  transfersyntax->buf = calloc(2, sizeof(uint8_t));
  transfersyntax->buf[0] = 0x51;
  transfersyntax->buf[1] = 0x01;

  pdvResultList->list.array[0]->transfersyntaxname = transfersyntax;

  pdvResultList->list.array[1] = calloc(1, sizeof(ContextDefResListSeq_t));

	pdvResultList->list.array[1]->result = result_acceptance;

  pdvResultList->list.array[1]->providerreason = 0;

  transfersyntax = calloc(1, sizeof(OBJECT_IDENTIFIER_t));

	transfersyntax->size = 2;
	transfersyntax->buf = calloc(2, sizeof(uint8_t));
	transfersyntax->buf[0] = 0x51;
	transfersyntax->buf[1] = 0x01;

	pdvResultList->list.array[1]->transfersyntaxname = transfersyntax;

  cpatype->normalModeParameters->protocolVersion = 0; //protocolversion;
	cpatype->normalModeParameters->respondingSelector = respondingSelector;
  cpatype->normalModeParameters->contextDefResList = pdvResultList;
  cpatype->normalModeParameters->requirements = 0;
  cpatype->normalModeParameters->userSessionRequirements = 0;

  Userdata_t* userdata = calloc(1, sizeof(Userdata_t));

  userdata->present = Userdata_PR_fullyencodeddata;
  userdata->choice.fullyencodeddata.list.count=1;
  userdata->choice.fullyencodeddata.list.array = calloc(1, sizeof(struct PDVlist*));

  struct PDVlist* pdvList = calloc(1, sizeof(struct PDVlist));

  pdvList->transfersyntaxname = 0;
  pdvList->presentationcontextidentifier = 1; /* 1 - ACSE */
  pdvList->presentationdatavalues.present = presentationdatavalues_PR_singleASN1type;

  /* Copy apdu buffer - required for free_struct to work properly */
	uint8_t* apduCopy = calloc(SBuffer_GetPayloadSize(sbuf), sizeof(uint8_t));
  memcpy(apduCopy, SBuffer_GetPayload(sbuf), SBuffer_GetPayloadSize(sbuf));

	pdvList->presentationdatavalues.choice.singleASN1type.size = SBuffer_GetPayloadSize(sbuf);
  pdvList->presentationdatavalues.choice.singleASN1type.buf = apduCopy;

  userdata->choice.fullyencodeddata.list.array[0] = pdvList;
  cpatype->normalModeParameters->userData = userdata;

	writer(&asn_DEF_CPAPPDU, cpatype, sbuf);

  asn_DEF_CPAPPDU.free_struct(&asn_DEF_CPAPPDU, cpatype, 0);
  return PRESENTATION_OK;
}


static void
	writer(asn_TYPE_descriptor_t * pxType, CPAPPDU_t* pxCpappdu, SBuffer * pxSbuf) {
	
	//pxSbuf->ulFrontLenght = 0;
	der_encode(pxType, pxCpappdu, (asn_app_consume_bytes_f *)SBuffer_Writer, (void*) pxSbuf);
	
	// т.к der_encode записывает в начало массива помимо заголовка ppdu еще и
	// содержимое acse+mms, которое уже находилось в массиве в конце, то нужно
	// просто представить, что массив пустой и толкать данные в самый конец, 
	// затирая предыдущие данные
	SBuffer_Replace(pxSbuf);
}

