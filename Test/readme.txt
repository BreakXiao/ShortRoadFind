���Ի����ṹ����

|\.
|--bin
   |--GraphBuild.exe  //���ɲ������ݵ�Ӧ�ó���
   |--FindRoad.exe    //��Ѱ����ĳ���

|--conf
   |--GraphInfo.txt   //���ò������ݵ���������

|--data               //dataĿ¼����������ļ�        
   |--topo.csv        
   |--demand.csv
   |--result.csv      //�ο���
   |--myresult.csv    //ʹ��Ѱ·�㷨�ѵõĲ��Դ�

|--script
  |--BuildGraph.bat   //�����������ݵ�����
  |--FindRoad.bat     //�����㷨��·

|--log  
   |--testlog.log     //Ѱ·��־


ʹ��˵����
��scriptĿ¼�н��в��Լ��ɡ�
1.����confĿ¼������GraphInfo.conf�ļ���
2.����˫��script�µ�BuildGraph.bat����������ͼ�ļ�topo�������ļ�demand���Լ��ο���result�� �����ʾ����999999˵������ʧ�ܣ���Ϊ��bugû����á���������һ�Ρ�

3.˫��FindRoad.bat���������ɽ��myresult.csv��dataĿ¼�¡������־��logĿ¼��
