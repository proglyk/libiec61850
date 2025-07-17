#ifndef _LIBIEC61850_CONF_H_
#define _LIBIEC61850_CONF_H_

#define LIBIEC61850_VERSION	"0.3.3.0"	

// ISO_CONNECTION
// ����� ������
#define RECEIVE_BUF_SIZE 256
// ����� ��������. ����� ������� SRAM2
#define SEND_BUF_SIZE 0x4000

// COTPCONNECTION
#define COTP_MAX_TPDU_SIZE (int)(1024+7)
#define COTP_DATA_HEADER_SIZE 3
#define COTP_RFC1006_HEADER_SIZE 4

// ����. ����� ���� ���������� � ������. ��� ��� �������, ������� ����� � 
// readAccessGooseControlBlock � ������������ ����� ����� ������ variableId, 
// ����������� ������ ��� ������ ���������. ��� ������ �����������, �.�. ����-
// ������� ������� �������, ������������ malloc, ����� ������ � ������� 2 �����
// �� �����! (������ ����� ����� ������� "GO", ������� ��������, ����� separator
// ���������� "*separator = 0")
#define MAX_VAR_PATH_LENGHT (long)64

#endif /*_LIBIEC61850_CONF_H_*/