/*
 * static_model.c
 *
 * automatically generated from Ver2.ed9.1.scd
 */
#include <stdlib.h>
#include "libiec61850/api/model.h"

IedModel iedModel;
static DataSetPtr datasets[];
static ReportControlBlockPtr reportControlBlocks[];
static GSEControlBlockPtr gseControlBlocks[];
static void initializeValues();
LogicalDevice iedModel_UPG;
LogicalNode   iedModel_UPG_LLN01;
DataObject    iedModel_UPG_LLN01_Beh;
DataAttribute iedModel_UPG_LLN01_Beh_q;
DataAttribute iedModel_UPG_LLN01_Beh_stVal;
DataAttribute iedModel_UPG_LLN01_Beh_t;
LogicalNode   iedModel_UPG_LPHD1;
DataObject    iedModel_UPG_LPHD1_PhyNam;
DataAttribute iedModel_UPG_LPHD1_PhyNam_vendor;
DataObject    iedModel_UPG_LPHD1_PhyHealth;
DataAttribute iedModel_UPG_LPHD1_PhyHealth_q;
DataAttribute iedModel_UPG_LPHD1_PhyHealth_stVal;
DataAttribute iedModel_UPG_LPHD1_PhyHealth_t;
DataObject    iedModel_UPG_LPHD1_Proxy;
DataAttribute iedModel_UPG_LPHD1_Proxy_q;
DataAttribute iedModel_UPG_LPHD1_Proxy_stVal;
DataAttribute iedModel_UPG_LPHD1_Proxy_t;
DataObject    iedModel_UPG_LPHD1_Beh;
DataAttribute iedModel_UPG_LPHD1_Beh_q;
DataAttribute iedModel_UPG_LPHD1_Beh_stVal;
DataAttribute iedModel_UPG_LPHD1_Beh_t;
LogicalNode   iedModel_UPG_GGIO0;
DataObject    iedModel_UPG_GGIO0_Beh;
DataAttribute iedModel_UPG_GGIO0_Beh_q;
DataAttribute iedModel_UPG_GGIO0_Beh_stVal;
DataAttribute iedModel_UPG_GGIO0_Beh_t;
DataObject    iedModel_UPG_GGIO0_ConnStatus;
DataAttribute iedModel_UPG_GGIO0_ConnStatus_mag;
DataAttribute iedModel_UPG_GGIO0_ConnStatus_mag_i;
DataAttribute iedModel_UPG_GGIO0_ConnStatus_q;
DataAttribute iedModel_UPG_GGIO0_ConnStatus_t;
DataObject    iedModel_UPG_GGIO0_HV;
DataAttribute iedModel_UPG_GGIO0_HV_mag;
DataAttribute iedModel_UPG_GGIO0_HV_mag_i;
DataAttribute iedModel_UPG_GGIO0_HV_q;
DataAttribute iedModel_UPG_GGIO0_HV_t;
DataObject    iedModel_UPG_GGIO0_LV;
DataAttribute iedModel_UPG_GGIO0_LV_mag;
DataAttribute iedModel_UPG_GGIO0_LV_mag_i;
DataAttribute iedModel_UPG_GGIO0_LV_q;
DataAttribute iedModel_UPG_GGIO0_LV_t;
DataObject    iedModel_UPG_GGIO0_Pause;
DataAttribute iedModel_UPG_GGIO0_Pause_mag;
DataAttribute iedModel_UPG_GGIO0_Pause_mag_i;
DataAttribute iedModel_UPG_GGIO0_Pause_q;
DataAttribute iedModel_UPG_GGIO0_Pause_t;
DataObject    iedModel_UPG_GGIO0_Protect1;
DataAttribute iedModel_UPG_GGIO0_Protect1_mag;
DataAttribute iedModel_UPG_GGIO0_Protect1_mag_i;
DataAttribute iedModel_UPG_GGIO0_Protect1_q;
DataAttribute iedModel_UPG_GGIO0_Protect1_t;
DataObject    iedModel_UPG_GGIO0_Protect2;
DataAttribute iedModel_UPG_GGIO0_Protect2_mag;
DataAttribute iedModel_UPG_GGIO0_Protect2_mag_i;
DataAttribute iedModel_UPG_GGIO0_Protect2_q;
DataAttribute iedModel_UPG_GGIO0_Protect2_t;
DataObject    iedModel_UPG_GGIO0_Ready;
DataAttribute iedModel_UPG_GGIO0_Ready_mag;
DataAttribute iedModel_UPG_GGIO0_Ready_mag_i;
DataAttribute iedModel_UPG_GGIO0_Ready_q;
DataAttribute iedModel_UPG_GGIO0_Ready_t;
DataObject    iedModel_UPG_GGIO0_Work;
DataAttribute iedModel_UPG_GGIO0_Work_mag;
DataAttribute iedModel_UPG_GGIO0_Work_mag_i;
DataAttribute iedModel_UPG_GGIO0_Work_q;
DataAttribute iedModel_UPG_GGIO0_Work_t;
DataObject    iedModel_UPG_GGIO0_Id;
DataAttribute iedModel_UPG_GGIO0_Id_mag;
DataAttribute iedModel_UPG_GGIO0_Id_mag_f;
DataAttribute iedModel_UPG_GGIO0_Id_q;
DataAttribute iedModel_UPG_GGIO0_Id_t;
DataObject    iedModel_UPG_GGIO0_Rl;
DataAttribute iedModel_UPG_GGIO0_Rl_mag;
DataAttribute iedModel_UPG_GGIO0_Rl_mag_f;
DataAttribute iedModel_UPG_GGIO0_Rl_q;
DataAttribute iedModel_UPG_GGIO0_Rl_t;
DataObject    iedModel_UPG_GGIO0_Ud;
DataAttribute iedModel_UPG_GGIO0_Ud_mag;
DataAttribute iedModel_UPG_GGIO0_Ud_mag_f;
DataAttribute iedModel_UPG_GGIO0_Ud_q;
DataAttribute iedModel_UPG_GGIO0_Ud_t;
DataObject    iedModel_UPG_GGIO0_Circ;
DataAttribute iedModel_UPG_GGIO0_Circ_mag;
DataAttribute iedModel_UPG_GGIO0_Circ_mag_i;
DataAttribute iedModel_UPG_GGIO0_Circ_q;
DataAttribute iedModel_UPG_GGIO0_Circ_t;
DataObject    iedModel_UPG_GGIO0_Iz;
DataAttribute iedModel_UPG_GGIO0_Iz_mag;
DataAttribute iedModel_UPG_GGIO0_Iz_mag_f;
DataAttribute iedModel_UPG_GGIO0_Iz_q;
DataAttribute iedModel_UPG_GGIO0_Iz_t;
DataObject    iedModel_UPG_GGIO0_Rlz;
DataAttribute iedModel_UPG_GGIO0_Rlz_mag;
DataAttribute iedModel_UPG_GGIO0_Rlz_mag_f;
DataAttribute iedModel_UPG_GGIO0_Rlz_q;
DataAttribute iedModel_UPG_GGIO0_Rlz_t;
LogicalNode   iedModel_UPG_GGIO1;
DataObject    iedModel_UPG_GGIO1_Beh;
DataAttribute iedModel_UPG_GGIO1_Beh_q;
DataAttribute iedModel_UPG_GGIO1_Beh_stVal;
DataAttribute iedModel_UPG_GGIO1_Beh_t;
DataObject    iedModel_UPG_GGIO1_Id;
DataAttribute iedModel_UPG_GGIO1_Id_mag;
DataAttribute iedModel_UPG_GGIO1_Id_mag_f;
DataAttribute iedModel_UPG_GGIO1_Id_q;
DataAttribute iedModel_UPG_GGIO1_Id_t;
DataObject    iedModel_UPG_GGIO1_Ud;
DataAttribute iedModel_UPG_GGIO1_Ud_mag;
DataAttribute iedModel_UPG_GGIO1_Ud_mag_f;
DataAttribute iedModel_UPG_GGIO1_Ud_q;
DataAttribute iedModel_UPG_GGIO1_Ud_t;
DataObject    iedModel_UPG_GGIO1_Iz;
DataAttribute iedModel_UPG_GGIO1_Iz_mag;
DataAttribute iedModel_UPG_GGIO1_Iz_mag_f;
DataAttribute iedModel_UPG_GGIO1_Iz_q;
DataAttribute iedModel_UPG_GGIO1_Iz_t;
DataObject    iedModel_UPG_GGIO1_PKZ;
DataAttribute iedModel_UPG_GGIO1_PKZ_mag;
DataAttribute iedModel_UPG_GGIO1_PKZ_mag_i;
DataAttribute iedModel_UPG_GGIO1_PKZ_q;
DataAttribute iedModel_UPG_GGIO1_PKZ_t;
DataObject    iedModel_UPG_GGIO1_POS;
DataAttribute iedModel_UPG_GGIO1_POS_mag;
DataAttribute iedModel_UPG_GGIO1_POS_mag_i;
DataAttribute iedModel_UPG_GGIO1_POS_q;
DataAttribute iedModel_UPG_GGIO1_POS_t;
DataObject    iedModel_UPG_GGIO1_SMU;
DataAttribute iedModel_UPG_GGIO1_SMU_mag;
DataAttribute iedModel_UPG_GGIO1_SMU_mag_i;
DataAttribute iedModel_UPG_GGIO1_SMU_q;
DataAttribute iedModel_UPG_GGIO1_SMU_t;
DataObject    iedModel_UPG_GGIO1_TAminus;
DataAttribute iedModel_UPG_GGIO1_TAminus_mag;
DataAttribute iedModel_UPG_GGIO1_TAminus_mag_i;
DataAttribute iedModel_UPG_GGIO1_TAminus_q;
DataAttribute iedModel_UPG_GGIO1_TAminus_t;
DataObject    iedModel_UPG_GGIO1_TAplus;
DataAttribute iedModel_UPG_GGIO1_TAplus_mag;
DataAttribute iedModel_UPG_GGIO1_TAplus_mag_i;
DataAttribute iedModel_UPG_GGIO1_TAplus_q;
DataAttribute iedModel_UPG_GGIO1_TAplus_t;
DataObject    iedModel_UPG_GGIO1_TBminus;
DataAttribute iedModel_UPG_GGIO1_TBminus_mag;
DataAttribute iedModel_UPG_GGIO1_TBminus_mag_i;
DataAttribute iedModel_UPG_GGIO1_TBminus_q;
DataAttribute iedModel_UPG_GGIO1_TBminus_t;
DataObject    iedModel_UPG_GGIO1_TBplus;
DataAttribute iedModel_UPG_GGIO1_TBplus_mag;
DataAttribute iedModel_UPG_GGIO1_TBplus_mag_i;
DataAttribute iedModel_UPG_GGIO1_TBplus_q;
DataAttribute iedModel_UPG_GGIO1_TBplus_t;
DataObject    iedModel_UPG_GGIO1_TCminus;
DataAttribute iedModel_UPG_GGIO1_TCminus_mag;
DataAttribute iedModel_UPG_GGIO1_TCminus_mag_i;
DataAttribute iedModel_UPG_GGIO1_TCminus_q;
DataAttribute iedModel_UPG_GGIO1_TCminus_t;
DataObject    iedModel_UPG_GGIO1_TCplus;
DataAttribute iedModel_UPG_GGIO1_TCplus_mag;
DataAttribute iedModel_UPG_GGIO1_TCplus_mag_i;
DataAttribute iedModel_UPG_GGIO1_TCplus_q;
DataAttribute iedModel_UPG_GGIO1_TCplus_t;

IedModel iedModel = {
    "IED",
    &iedModel_UPG,
    datasets,
    reportControlBlocks,
    gseControlBlocks,
    initializeValues
};

LogicalDevice iedModel_UPG = {
    "IEDUPG",
    NULL,
    &iedModel_UPG_LLN01
};

LogicalNode iedModel_UPG_LLN01 = {
    LogicalNodeModelType,
    "LLN01",
    &iedModel_UPG,
    (ModelNode*) &iedModel_UPG_LPHD1,
    (ModelNode*) &iedModel_UPG_LLN01_Beh,
};

DataObject iedModel_UPG_LLN01_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_UPG_LLN01,
    NULL,
    (ModelNode*) &iedModel_UPG_LLN01_Beh_q,
    0,
    0
};

DataAttribute iedModel_UPG_LLN01_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_LLN01_Beh,
    (ModelNode*) &iedModel_UPG_LLN01_Beh_stVal,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_LLN01_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_UPG_LLN01_Beh,
    (ModelNode*) &iedModel_UPG_LLN01_Beh_t,
    NULL,
    0,
    0,
    ST,
    ENUMERATED,
    NULL
};

DataAttribute iedModel_UPG_LLN01_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_LLN01_Beh,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

LogicalNode iedModel_UPG_LPHD1 = {
    LogicalNodeModelType,
    "LPHD1",
    &iedModel_UPG,
    (ModelNode*) &iedModel_UPG_GGIO0,
    (ModelNode*) &iedModel_UPG_LPHD1_PhyNam,
};

DataObject iedModel_UPG_LPHD1_PhyNam = {
    DataObjectModelType,
    "PhyNam",
    (ModelNode*) &iedModel_UPG_LPHD1,
    (ModelNode*) &iedModel_UPG_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_UPG_LPHD1_PhyNam_vendor,
    0,
    0
};

DataAttribute iedModel_UPG_LPHD1_PhyNam_vendor = {
    DataAttributeModelType,
    "vendor",
    (ModelNode*) &iedModel_UPG_LPHD1_PhyNam,
    NULL,
    NULL,
    0,
    0,
    DC,
    VISIBLE_STRING_255,
    NULL
};

DataObject iedModel_UPG_LPHD1_PhyHealth = {
    DataObjectModelType,
    "PhyHealth",
    (ModelNode*) &iedModel_UPG_LPHD1,
    (ModelNode*) &iedModel_UPG_LPHD1_Proxy,
    (ModelNode*) &iedModel_UPG_LPHD1_PhyHealth_q,
    0,
    0
};

DataAttribute iedModel_UPG_LPHD1_PhyHealth_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_UPG_LPHD1_PhyHealth_stVal,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_LPHD1_PhyHealth_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_UPG_LPHD1_PhyHealth,
    (ModelNode*) &iedModel_UPG_LPHD1_PhyHealth_t,
    NULL,
    0,
    0,
    ST,
    ENUMERATED,
    NULL
};

DataAttribute iedModel_UPG_LPHD1_PhyHealth_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_LPHD1_PhyHealth,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_LPHD1_Proxy = {
    DataObjectModelType,
    "Proxy",
    (ModelNode*) &iedModel_UPG_LPHD1,
    (ModelNode*) &iedModel_UPG_LPHD1_Beh,
    (ModelNode*) &iedModel_UPG_LPHD1_Proxy_q,
    0,
    0
};

DataAttribute iedModel_UPG_LPHD1_Proxy_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_LPHD1_Proxy,
    (ModelNode*) &iedModel_UPG_LPHD1_Proxy_stVal,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_LPHD1_Proxy_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_UPG_LPHD1_Proxy,
    (ModelNode*) &iedModel_UPG_LPHD1_Proxy_t,
    NULL,
    0,
    0,
    ST,
    BOOLEAN,
    NULL
};

DataAttribute iedModel_UPG_LPHD1_Proxy_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_LPHD1_Proxy,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_LPHD1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_UPG_LPHD1,
    NULL,
    (ModelNode*) &iedModel_UPG_LPHD1_Beh_q,
    0,
    0
};

DataAttribute iedModel_UPG_LPHD1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_LPHD1_Beh,
    (ModelNode*) &iedModel_UPG_LPHD1_Beh_stVal,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_LPHD1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_UPG_LPHD1_Beh,
    (ModelNode*) &iedModel_UPG_LPHD1_Beh_t,
    NULL,
    0,
    0,
    ST,
    ENUMERATED,
    NULL
};

DataAttribute iedModel_UPG_LPHD1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_LPHD1_Beh,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

LogicalNode iedModel_UPG_GGIO0 = {
    LogicalNodeModelType,
    "GGIO0",
    &iedModel_UPG,
    (ModelNode*) &iedModel_UPG_GGIO1,
    (ModelNode*) &iedModel_UPG_GGIO0_Beh,
};

DataObject iedModel_UPG_GGIO0_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_UPG_GGIO0,
    (ModelNode*) &iedModel_UPG_GGIO0_ConnStatus,
    (ModelNode*) &iedModel_UPG_GGIO0_Beh_q,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO0_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO0_Beh,
    (ModelNode*) &iedModel_UPG_GGIO0_Beh_stVal,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_UPG_GGIO0_Beh,
    (ModelNode*) &iedModel_UPG_GGIO0_Beh_t,
    NULL,
    0,
    0,
    ST,
    ENUMERATED,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO0_Beh,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO0_ConnStatus = {
    DataObjectModelType,
    "ConnStatus",
    (ModelNode*) &iedModel_UPG_GGIO0,
    (ModelNode*) &iedModel_UPG_GGIO0_HV,
    (ModelNode*) &iedModel_UPG_GGIO0_ConnStatus_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO0_ConnStatus_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO0_ConnStatus,
    (ModelNode*) &iedModel_UPG_GGIO0_ConnStatus_q,
    (ModelNode*) &iedModel_UPG_GGIO0_ConnStatus_mag_i,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_ConnStatus_mag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_UPG_GGIO0_ConnStatus_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    INT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_ConnStatus_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO0_ConnStatus,
    (ModelNode*) &iedModel_UPG_GGIO0_ConnStatus_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_ConnStatus_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO0_ConnStatus,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO0_HV = {
    DataObjectModelType,
    "HV",
    (ModelNode*) &iedModel_UPG_GGIO0,
    (ModelNode*) &iedModel_UPG_GGIO0_LV,
    (ModelNode*) &iedModel_UPG_GGIO0_HV_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO0_HV_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO0_HV,
    (ModelNode*) &iedModel_UPG_GGIO0_HV_q,
    (ModelNode*) &iedModel_UPG_GGIO0_HV_mag_i,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_HV_mag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_UPG_GGIO0_HV_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    INT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_HV_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO0_HV,
    (ModelNode*) &iedModel_UPG_GGIO0_HV_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_HV_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO0_HV,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO0_LV = {
    DataObjectModelType,
    "LV",
    (ModelNode*) &iedModel_UPG_GGIO0,
    (ModelNode*) &iedModel_UPG_GGIO0_Pause,
    (ModelNode*) &iedModel_UPG_GGIO0_LV_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO0_LV_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO0_LV,
    (ModelNode*) &iedModel_UPG_GGIO0_LV_q,
    (ModelNode*) &iedModel_UPG_GGIO0_LV_mag_i,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_LV_mag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_UPG_GGIO0_LV_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    INT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_LV_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO0_LV,
    (ModelNode*) &iedModel_UPG_GGIO0_LV_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_LV_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO0_LV,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO0_Pause = {
    DataObjectModelType,
    "Pause",
    (ModelNode*) &iedModel_UPG_GGIO0,
    (ModelNode*) &iedModel_UPG_GGIO0_Protect1,
    (ModelNode*) &iedModel_UPG_GGIO0_Pause_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO0_Pause_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO0_Pause,
    (ModelNode*) &iedModel_UPG_GGIO0_Pause_q,
    (ModelNode*) &iedModel_UPG_GGIO0_Pause_mag_i,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Pause_mag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_UPG_GGIO0_Pause_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    INT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Pause_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO0_Pause,
    (ModelNode*) &iedModel_UPG_GGIO0_Pause_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Pause_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO0_Pause,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO0_Protect1 = {
    DataObjectModelType,
    "Protect1",
    (ModelNode*) &iedModel_UPG_GGIO0,
    (ModelNode*) &iedModel_UPG_GGIO0_Protect2,
    (ModelNode*) &iedModel_UPG_GGIO0_Protect1_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO0_Protect1_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO0_Protect1,
    (ModelNode*) &iedModel_UPG_GGIO0_Protect1_q,
    (ModelNode*) &iedModel_UPG_GGIO0_Protect1_mag_i,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Protect1_mag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_UPG_GGIO0_Protect1_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    INT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Protect1_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO0_Protect1,
    (ModelNode*) &iedModel_UPG_GGIO0_Protect1_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Protect1_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO0_Protect1,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO0_Protect2 = {
    DataObjectModelType,
    "Protect2",
    (ModelNode*) &iedModel_UPG_GGIO0,
    (ModelNode*) &iedModel_UPG_GGIO0_Ready,
    (ModelNode*) &iedModel_UPG_GGIO0_Protect2_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO0_Protect2_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO0_Protect2,
    (ModelNode*) &iedModel_UPG_GGIO0_Protect2_q,
    (ModelNode*) &iedModel_UPG_GGIO0_Protect2_mag_i,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Protect2_mag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_UPG_GGIO0_Protect2_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    INT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Protect2_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO0_Protect2,
    (ModelNode*) &iedModel_UPG_GGIO0_Protect2_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Protect2_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO0_Protect2,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO0_Ready = {
    DataObjectModelType,
    "Ready",
    (ModelNode*) &iedModel_UPG_GGIO0,
    (ModelNode*) &iedModel_UPG_GGIO0_Work,
    (ModelNode*) &iedModel_UPG_GGIO0_Ready_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO0_Ready_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO0_Ready,
    (ModelNode*) &iedModel_UPG_GGIO0_Ready_q,
    (ModelNode*) &iedModel_UPG_GGIO0_Ready_mag_i,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Ready_mag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_UPG_GGIO0_Ready_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    INT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Ready_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO0_Ready,
    (ModelNode*) &iedModel_UPG_GGIO0_Ready_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Ready_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO0_Ready,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO0_Work = {
    DataObjectModelType,
    "Work",
    (ModelNode*) &iedModel_UPG_GGIO0,
    (ModelNode*) &iedModel_UPG_GGIO0_Id,
    (ModelNode*) &iedModel_UPG_GGIO0_Work_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO0_Work_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO0_Work,
    (ModelNode*) &iedModel_UPG_GGIO0_Work_q,
    (ModelNode*) &iedModel_UPG_GGIO0_Work_mag_i,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Work_mag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_UPG_GGIO0_Work_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    INT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Work_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO0_Work,
    (ModelNode*) &iedModel_UPG_GGIO0_Work_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Work_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO0_Work,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO0_Id = {
    DataObjectModelType,
    "Id",
    (ModelNode*) &iedModel_UPG_GGIO0,
    (ModelNode*) &iedModel_UPG_GGIO0_Rl,
    (ModelNode*) &iedModel_UPG_GGIO0_Id_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO0_Id_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO0_Id,
    (ModelNode*) &iedModel_UPG_GGIO0_Id_q,
    (ModelNode*) &iedModel_UPG_GGIO0_Id_mag_f,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Id_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_UPG_GGIO0_Id_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    FLOAT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Id_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO0_Id,
    (ModelNode*) &iedModel_UPG_GGIO0_Id_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Id_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO0_Id,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO0_Rl = {
    DataObjectModelType,
    "Rl",
    (ModelNode*) &iedModel_UPG_GGIO0,
    (ModelNode*) &iedModel_UPG_GGIO0_Ud,
    (ModelNode*) &iedModel_UPG_GGIO0_Rl_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO0_Rl_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO0_Rl,
    (ModelNode*) &iedModel_UPG_GGIO0_Rl_q,
    (ModelNode*) &iedModel_UPG_GGIO0_Rl_mag_f,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Rl_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_UPG_GGIO0_Rl_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    FLOAT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Rl_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO0_Rl,
    (ModelNode*) &iedModel_UPG_GGIO0_Rl_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Rl_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO0_Rl,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO0_Ud = {
    DataObjectModelType,
    "Ud",
    (ModelNode*) &iedModel_UPG_GGIO0,
    (ModelNode*) &iedModel_UPG_GGIO0_Circ,
    (ModelNode*) &iedModel_UPG_GGIO0_Ud_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO0_Ud_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO0_Ud,
    (ModelNode*) &iedModel_UPG_GGIO0_Ud_q,
    (ModelNode*) &iedModel_UPG_GGIO0_Ud_mag_f,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Ud_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_UPG_GGIO0_Ud_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    FLOAT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Ud_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO0_Ud,
    (ModelNode*) &iedModel_UPG_GGIO0_Ud_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Ud_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO0_Ud,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO0_Circ = {
    DataObjectModelType,
    "Circ",
    (ModelNode*) &iedModel_UPG_GGIO0,
    (ModelNode*) &iedModel_UPG_GGIO0_Iz,
    (ModelNode*) &iedModel_UPG_GGIO0_Circ_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO0_Circ_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO0_Circ,
    (ModelNode*) &iedModel_UPG_GGIO0_Circ_q,
    (ModelNode*) &iedModel_UPG_GGIO0_Circ_mag_i,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Circ_mag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_UPG_GGIO0_Circ_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    INT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Circ_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO0_Circ,
    (ModelNode*) &iedModel_UPG_GGIO0_Circ_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Circ_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO0_Circ,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO0_Iz = {
    DataObjectModelType,
    "Iz",
    (ModelNode*) &iedModel_UPG_GGIO0,
    (ModelNode*) &iedModel_UPG_GGIO0_Rlz,
    (ModelNode*) &iedModel_UPG_GGIO0_Iz_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO0_Iz_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO0_Iz,
    (ModelNode*) &iedModel_UPG_GGIO0_Iz_q,
    (ModelNode*) &iedModel_UPG_GGIO0_Iz_mag_f,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Iz_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_UPG_GGIO0_Iz_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    FLOAT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Iz_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO0_Iz,
    (ModelNode*) &iedModel_UPG_GGIO0_Iz_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Iz_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO0_Iz,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO0_Rlz = {
    DataObjectModelType,
    "Rlz",
    (ModelNode*) &iedModel_UPG_GGIO0,
    NULL,
    (ModelNode*) &iedModel_UPG_GGIO0_Rlz_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO0_Rlz_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO0_Rlz,
    (ModelNode*) &iedModel_UPG_GGIO0_Rlz_q,
    (ModelNode*) &iedModel_UPG_GGIO0_Rlz_mag_f,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Rlz_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_UPG_GGIO0_Rlz_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    FLOAT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Rlz_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO0_Rlz,
    (ModelNode*) &iedModel_UPG_GGIO0_Rlz_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO0_Rlz_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO0_Rlz,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

LogicalNode iedModel_UPG_GGIO1 = {
    LogicalNodeModelType,
    "GGIO1",
    &iedModel_UPG,
    NULL,
    (ModelNode*) &iedModel_UPG_GGIO1_Beh,
};

DataObject iedModel_UPG_GGIO1_Beh = {
    DataObjectModelType,
    "Beh",
    (ModelNode*) &iedModel_UPG_GGIO1,
    (ModelNode*) &iedModel_UPG_GGIO1_Id,
    (ModelNode*) &iedModel_UPG_GGIO1_Beh_q,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO1_Beh_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO1_Beh,
    (ModelNode*) &iedModel_UPG_GGIO1_Beh_stVal,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_Beh_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_UPG_GGIO1_Beh,
    (ModelNode*) &iedModel_UPG_GGIO1_Beh_t,
    NULL,
    0,
    0,
    ST,
    ENUMERATED,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_Beh_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO1_Beh,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO1_Id = {
    DataObjectModelType,
    "Id",
    (ModelNode*) &iedModel_UPG_GGIO1,
    (ModelNode*) &iedModel_UPG_GGIO1_Ud,
    (ModelNode*) &iedModel_UPG_GGIO1_Id_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO1_Id_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO1_Id,
    (ModelNode*) &iedModel_UPG_GGIO1_Id_q,
    (ModelNode*) &iedModel_UPG_GGIO1_Id_mag_f,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_Id_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_UPG_GGIO1_Id_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    FLOAT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_Id_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO1_Id,
    (ModelNode*) &iedModel_UPG_GGIO1_Id_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_Id_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO1_Id,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO1_Ud = {
    DataObjectModelType,
    "Ud",
    (ModelNode*) &iedModel_UPG_GGIO1,
    (ModelNode*) &iedModel_UPG_GGIO1_Iz,
    (ModelNode*) &iedModel_UPG_GGIO1_Ud_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO1_Ud_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO1_Ud,
    (ModelNode*) &iedModel_UPG_GGIO1_Ud_q,
    (ModelNode*) &iedModel_UPG_GGIO1_Ud_mag_f,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_Ud_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_UPG_GGIO1_Ud_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    FLOAT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_Ud_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO1_Ud,
    (ModelNode*) &iedModel_UPG_GGIO1_Ud_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_Ud_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO1_Ud,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO1_Iz = {
    DataObjectModelType,
    "Iz",
    (ModelNode*) &iedModel_UPG_GGIO1,
    (ModelNode*) &iedModel_UPG_GGIO1_PKZ,
    (ModelNode*) &iedModel_UPG_GGIO1_Iz_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO1_Iz_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO1_Iz,
    (ModelNode*) &iedModel_UPG_GGIO1_Iz_q,
    (ModelNode*) &iedModel_UPG_GGIO1_Iz_mag_f,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_Iz_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_UPG_GGIO1_Iz_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    FLOAT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_Iz_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO1_Iz,
    (ModelNode*) &iedModel_UPG_GGIO1_Iz_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_Iz_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO1_Iz,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO1_PKZ = {
    DataObjectModelType,
    "PKZ",
    (ModelNode*) &iedModel_UPG_GGIO1,
    (ModelNode*) &iedModel_UPG_GGIO1_POS,
    (ModelNode*) &iedModel_UPG_GGIO1_PKZ_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO1_PKZ_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO1_PKZ,
    (ModelNode*) &iedModel_UPG_GGIO1_PKZ_q,
    (ModelNode*) &iedModel_UPG_GGIO1_PKZ_mag_i,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_PKZ_mag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_UPG_GGIO1_PKZ_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    INT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_PKZ_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO1_PKZ,
    (ModelNode*) &iedModel_UPG_GGIO1_PKZ_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_PKZ_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO1_PKZ,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO1_POS = {
    DataObjectModelType,
    "POS",
    (ModelNode*) &iedModel_UPG_GGIO1,
    (ModelNode*) &iedModel_UPG_GGIO1_SMU,
    (ModelNode*) &iedModel_UPG_GGIO1_POS_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO1_POS_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO1_POS,
    (ModelNode*) &iedModel_UPG_GGIO1_POS_q,
    (ModelNode*) &iedModel_UPG_GGIO1_POS_mag_i,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_POS_mag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_UPG_GGIO1_POS_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    INT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_POS_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO1_POS,
    (ModelNode*) &iedModel_UPG_GGIO1_POS_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_POS_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO1_POS,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO1_SMU = {
    DataObjectModelType,
    "SMU",
    (ModelNode*) &iedModel_UPG_GGIO1,
    (ModelNode*) &iedModel_UPG_GGIO1_TAminus,
    (ModelNode*) &iedModel_UPG_GGIO1_SMU_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO1_SMU_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO1_SMU,
    (ModelNode*) &iedModel_UPG_GGIO1_SMU_q,
    (ModelNode*) &iedModel_UPG_GGIO1_SMU_mag_i,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_SMU_mag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_UPG_GGIO1_SMU_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    INT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_SMU_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO1_SMU,
    (ModelNode*) &iedModel_UPG_GGIO1_SMU_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_SMU_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO1_SMU,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO1_TAminus = {
    DataObjectModelType,
    "TAminus",
    (ModelNode*) &iedModel_UPG_GGIO1,
    (ModelNode*) &iedModel_UPG_GGIO1_TAplus,
    (ModelNode*) &iedModel_UPG_GGIO1_TAminus_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO1_TAminus_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO1_TAminus,
    (ModelNode*) &iedModel_UPG_GGIO1_TAminus_q,
    (ModelNode*) &iedModel_UPG_GGIO1_TAminus_mag_i,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_TAminus_mag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_UPG_GGIO1_TAminus_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    INT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_TAminus_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO1_TAminus,
    (ModelNode*) &iedModel_UPG_GGIO1_TAminus_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_TAminus_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO1_TAminus,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO1_TAplus = {
    DataObjectModelType,
    "TAplus",
    (ModelNode*) &iedModel_UPG_GGIO1,
    (ModelNode*) &iedModel_UPG_GGIO1_TBminus,
    (ModelNode*) &iedModel_UPG_GGIO1_TAplus_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO1_TAplus_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO1_TAplus,
    (ModelNode*) &iedModel_UPG_GGIO1_TAplus_q,
    (ModelNode*) &iedModel_UPG_GGIO1_TAplus_mag_i,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_TAplus_mag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_UPG_GGIO1_TAplus_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    INT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_TAplus_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO1_TAplus,
    (ModelNode*) &iedModel_UPG_GGIO1_TAplus_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_TAplus_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO1_TAplus,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO1_TBminus = {
    DataObjectModelType,
    "TBminus",
    (ModelNode*) &iedModel_UPG_GGIO1,
    (ModelNode*) &iedModel_UPG_GGIO1_TBplus,
    (ModelNode*) &iedModel_UPG_GGIO1_TBminus_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO1_TBminus_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO1_TBminus,
    (ModelNode*) &iedModel_UPG_GGIO1_TBminus_q,
    (ModelNode*) &iedModel_UPG_GGIO1_TBminus_mag_i,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_TBminus_mag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_UPG_GGIO1_TBminus_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    INT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_TBminus_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO1_TBminus,
    (ModelNode*) &iedModel_UPG_GGIO1_TBminus_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_TBminus_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO1_TBminus,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO1_TBplus = {
    DataObjectModelType,
    "TBplus",
    (ModelNode*) &iedModel_UPG_GGIO1,
    (ModelNode*) &iedModel_UPG_GGIO1_TCminus,
    (ModelNode*) &iedModel_UPG_GGIO1_TBplus_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO1_TBplus_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO1_TBplus,
    (ModelNode*) &iedModel_UPG_GGIO1_TBplus_q,
    (ModelNode*) &iedModel_UPG_GGIO1_TBplus_mag_i,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_TBplus_mag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_UPG_GGIO1_TBplus_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    INT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_TBplus_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO1_TBplus,
    (ModelNode*) &iedModel_UPG_GGIO1_TBplus_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_TBplus_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO1_TBplus,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO1_TCminus = {
    DataObjectModelType,
    "TCminus",
    (ModelNode*) &iedModel_UPG_GGIO1,
    (ModelNode*) &iedModel_UPG_GGIO1_TCplus,
    (ModelNode*) &iedModel_UPG_GGIO1_TCminus_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO1_TCminus_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO1_TCminus,
    (ModelNode*) &iedModel_UPG_GGIO1_TCminus_q,
    (ModelNode*) &iedModel_UPG_GGIO1_TCminus_mag_i,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_TCminus_mag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_UPG_GGIO1_TCminus_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    INT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_TCminus_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO1_TCminus,
    (ModelNode*) &iedModel_UPG_GGIO1_TCminus_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_TCminus_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO1_TCminus,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

DataObject iedModel_UPG_GGIO1_TCplus = {
    DataObjectModelType,
    "TCplus",
    (ModelNode*) &iedModel_UPG_GGIO1,
    NULL,
    (ModelNode*) &iedModel_UPG_GGIO1_TCplus_mag,
    0,
    0
};

DataAttribute iedModel_UPG_GGIO1_TCplus_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_UPG_GGIO1_TCplus,
    (ModelNode*) &iedModel_UPG_GGIO1_TCplus_q,
    (ModelNode*) &iedModel_UPG_GGIO1_TCplus_mag_i,
    0,
    0,
    MX,
    CONSTRUCTED,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_TCplus_mag_i = {
    DataAttributeModelType,
    "i",
    (ModelNode*) &iedModel_UPG_GGIO1_TCplus_mag,
    NULL,
    NULL,
    0,
    0,
    MX,
    INT32,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_TCplus_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_UPG_GGIO1_TCplus,
    (ModelNode*) &iedModel_UPG_GGIO1_TCplus_t,
    NULL,
    0,
    0,
    ST,
    QUALITY,
    NULL
};

DataAttribute iedModel_UPG_GGIO1_TCplus_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_UPG_GGIO1_TCplus,
    NULL,
    NULL,
    0,
    0,
    ST,
    TIMESTAMP,
    NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetUPG1_fcda0 = {
  "IEDUPG",
  "GGIO1$MX$Id",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetUPG1_fcda1 = {
  "IEDUPG",
  "GGIO1$MX$Ud",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetUPG1_fcda2 = {
  "IEDUPG",
  "GGIO1$MX$Iz",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetUPG1_fcda3 = {
  "IEDUPG",
  "GGIO1$MX$PKZ",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetUPG1_fcda4 = {
  "IEDUPG",
  "GGIO1$MX$POS",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetUPG1_fcda5 = {
  "IEDUPG",
  "GGIO1$MX$SMU",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetUPG1_fcda6 = {
  "IEDUPG",
  "GGIO1$MX$TAminus",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetUPG1_fcda7 = {
  "IEDUPG",
  "GGIO1$MX$TAplus",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetUPG1_fcda8 = {
  "IEDUPG",
  "GGIO1$MX$TBminus",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetUPG1_fcda9 = {
  "IEDUPG",
  "GGIO1$MX$TBplus",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetUPG1_fcda10 = {
  "IEDUPG",
  "GGIO1$MX$TCminus",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetUPG1_fcda11 = {
  "IEDUPG",
  "GGIO1$MX$TCplus",
  -1,
  NULL,
  NULL
};

static DataSetEntry* ds_UPG_LLN01_DataSetUPG1_elements[12] = {
  &ds_UPG_LLN01_DataSetUPG1_fcda0,
  &ds_UPG_LLN01_DataSetUPG1_fcda1,
  &ds_UPG_LLN01_DataSetUPG1_fcda2,
  &ds_UPG_LLN01_DataSetUPG1_fcda3,
  &ds_UPG_LLN01_DataSetUPG1_fcda4,
  &ds_UPG_LLN01_DataSetUPG1_fcda5,
  &ds_UPG_LLN01_DataSetUPG1_fcda6,
  &ds_UPG_LLN01_DataSetUPG1_fcda7,
  &ds_UPG_LLN01_DataSetUPG1_fcda8,
  &ds_UPG_LLN01_DataSetUPG1_fcda9,
  &ds_UPG_LLN01_DataSetUPG1_fcda10,
  &ds_UPG_LLN01_DataSetUPG1_fcda11
};

static struct DataSet ds_UPG_LLN01_DataSetUPG1 = {
  "IEDUPG",
  "LLN01$DataSetUPG1",
  12,
  ds_UPG_LLN01_DataSetUPG1_elements
};
static DataSetEntry ds_UPG_LLN01_DataSetMain_fcda0 = {
  "IEDUPG",
  "GGIO0$MX$ConnStatus",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetMain_fcda1 = {
  "IEDUPG",
  "GGIO0$MX$HV",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetMain_fcda2 = {
  "IEDUPG",
  "GGIO0$MX$LV",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetMain_fcda3 = {
  "IEDUPG",
  "GGIO0$MX$Pause",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetMain_fcda4 = {
  "IEDUPG",
  "GGIO0$MX$Protect1",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetMain_fcda5 = {
  "IEDUPG",
  "GGIO0$MX$Protect2",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetMain_fcda6 = {
  "IEDUPG",
  "GGIO0$MX$Ready",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetMain_fcda7 = {
  "IEDUPG",
  "GGIO0$MX$Work",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetMain_fcda8 = {
  "IEDUPG",
  "GGIO0$MX$Id",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetMain_fcda9 = {
  "IEDUPG",
  "GGIO0$MX$Rl",
  -1,
  NULL,
  NULL
};

static DataSetEntry ds_UPG_LLN01_DataSetMain_fcda10 = {
  "IEDUPG",
  "GGIO0$MX$Ud",
  -1,
  NULL,
  NULL
};

static DataSetEntry* ds_UPG_LLN01_DataSetMain_elements[11] = {
  &ds_UPG_LLN01_DataSetMain_fcda0,
  &ds_UPG_LLN01_DataSetMain_fcda1,
  &ds_UPG_LLN01_DataSetMain_fcda2,
  &ds_UPG_LLN01_DataSetMain_fcda3,
  &ds_UPG_LLN01_DataSetMain_fcda4,
  &ds_UPG_LLN01_DataSetMain_fcda5,
  &ds_UPG_LLN01_DataSetMain_fcda6,
  &ds_UPG_LLN01_DataSetMain_fcda7,
  &ds_UPG_LLN01_DataSetMain_fcda8,
  &ds_UPG_LLN01_DataSetMain_fcda9,
  &ds_UPG_LLN01_DataSetMain_fcda10
};

static struct DataSet ds_UPG_LLN01_DataSetMain = {
  "IEDUPG",
  "LLN01$DataSetMain",
  11,
  ds_UPG_LLN01_DataSetMain_elements
};

static DataSetPtr datasets[] = {
  &ds_UPG_LLN01_DataSetUPG1,
  &ds_UPG_LLN01_DataSetMain,
  NULL,
};

static ReportControlBlockPtr reportControlBlocks[] = {
    NULL
};

static struct GSEControlBlock iedModel_UPG_LLN01_gse0 = {&iedModel_UPG_LLN01, "GCB1", "0x0000", "DataSetMain", 1, false};

static GSEControlBlockPtr gseControlBlocks[] = {
    &iedModel_UPG_LLN01_gse0,
    NULL
};


static void
initializeValues()
{

iedModel_UPG_LLN01_Beh_stVal.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_UPG_LPHD1_PhyHealth_stVal.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_UPG_LPHD1_Beh_stVal.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_UPG_GGIO0_Beh_stVal.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_UPG_GGIO1_Beh_stVal.mmsValue = MmsValue_newIntegerFromInt32(0);
}
