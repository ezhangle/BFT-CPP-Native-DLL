//��������ս��׷�ٵ�һ���֡�����������ָ������ѯս��1���ݲ������ݱ��浽ָ���ļ���
//ʹ��TRN��API����ȡս��1����:http://docs.trnbattlefield.apiary.io/#introduction/parameters/game
//ʹ��΢�� C++ REST SDK ������ HTTP ���󲢽����յ���JSON����:http://microsoft.github.io/cpprestsdk/

//�����������в���˵����ÿһ����һ�������Ŀ�ѡ��
//ID=Ҫ��ѯ��ID��
//GAME=��Ϸ������"tunguska"��
//STATS=��ѯ���ͣ���ѡ:BasicStats(����ͳ��)��DetailedStats(��ϸͳ��)��DogTagImg(����ͼƬURL)��Weapons(����ͳ��)��Vehicles(�ؾ�ͳ��)��KitRanks(���ֵȼ�)��
//SAVETO=���������ļ�����
//TRANSLATEFILE=�����ļ���(�������ν)��
//������ʾ��:BFT_M ID=liuziangexit GAME=tunguska STATS=BasicStats SAVETO=1.txt(��ѯliuziangexit��ս��1������Ϣ�����浽1.txt)

#pragma once
//��ַ������
#define BaseURI L"https://battlefieldtracker.com/bf1/api"
#define APIKEY L"40615011-4ab1-4da2-a7a9-7a8559ead70e"
#define LZAURI L"https://liuziangexit.com/BFT"
#define ServerAddrFilename L"ServerAddr.html"//�������ͱ��ض�������ļ���
#define TimeOut 30000
#define RecvLength 65535
#define BFT_Server_Response_Result_OK "OK"
#define BFT_Server_Response_Result_NO "NO"
#define ThisVersion "2000"
#define BFT_EXTERN_C  extern "C"  _declspec(dllexport)
//URI����
#define Platform L"platform"
#define Name L"displayName"
#define Game L"game"
#define PC L"3"
#define BF1 L"tunguska"
#define BF4 L"bf4"