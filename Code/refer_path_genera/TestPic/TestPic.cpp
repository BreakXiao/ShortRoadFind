#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<set>
#include<vector>
#include<queue>
#include<cmath>
#include<set>
using namespace std;

#define ELMax 20
#define NodeMax 2000
#define N 2000
#define Road_M 400050 
#define INF 99999999
struct Road//��·�ṹ��
{
	int s,e,l;  //s:·����ʼ�ˣ�e:·���նˣ�l:·��
	int next,up;   //����������next:��һ��·��up:��һ��·
}road[Road_M];
FILE *fdemand,*fans;
int Head[NodeMax];
int deflag[Road_M];
struct info
{
	int Start,End;
	int DemandNodeNum,FirstRoadNodeNum,SecondRoadNodeNum,AllNodeNum;
	int OtherNodeNum,MinOutDegree,MaxOutDegree;
};

struct TopoLine
{
	int i,s,e,l;
	int needn,nowlen;
};

struct  ShortRoad
{
	int dlen;
	int pointn;
	vector<int> PassRoad;
};



struct ShortRoad AnsRoad[NodeMax][NodeMax],MapRoad[NodeMax][NodeMax];
ShortRoad tmp[N][N];
void demandput(vector<int> ansvec,info PicInfo);

void information_input(struct info &PicInfo)
{
	FILE *fcin;
	fcin = fopen("..\\conf\\GraphInfo.txt","r");

	printf("ͼģ�����ɣ�\n");

	printf("���Ϊ0���յ�Ϊ1\n");
	PicInfo.Start=0;
	PicInfo.End=1;

	fscanf(fcin,"�ܽڵ���=%d\n",&PicInfo.AllNodeNum);	

	fscanf(fcin,"�ؾ�����=%d\n",&PicInfo.DemandNodeNum);
	fscanf(fcin,"��ѡ·�ϵ������ڵ���=%d\n",&PicInfo.FirstRoadNodeNum);
	fscanf(fcin,"��ѡ·�ϵ������ڵ���=%d\n",&PicInfo.SecondRoadNodeNum);
	fscanf(fcin,"�������С����=%d\n",&PicInfo.MinOutDegree);
	fscanf(fcin,"�����������=%d\n",&PicInfo.MaxOutDegree);

	printf("�ܽڵ���=%d\n",PicInfo.AllNodeNum);
	printf("�ؾ�����=%d\n",PicInfo.DemandNodeNum);
	printf("��ѡ·�ϵ������ڵ���=%d\n",PicInfo.FirstRoadNodeNum);
	printf("��ѡ·�ϵ������ڵ���=%d\n",PicInfo.SecondRoadNodeNum);
	printf("�������С����=%d\n",PicInfo.MinOutDegree);
	printf("�����������=%d\n",PicInfo.MaxOutDegree);
}

void setroad(TopoLine T)
{
	int i = T.i;
	road[i].s = T.s;
	road[i].e = T.e;
	road[i].l = T.l;
	road[i].next = Head[T.s];
	Head[T.s] = i;
}


int graph_build(struct info PicInfo,vector<TopoLine> &vecTopo)
{
	int i=0, OutDegree,Nodenum = PicInfo.AllNodeNum;
	queue<struct TopoLine> tq;
	struct TopoLine T,TT;
	for( i = 0; i < PicInfo.AllNodeNum; i++)
	{
		Head[i] = -1;
	}

	T.s = 0 ;
	tq.push(T);
	while(Nodenum--)
	{	
		if(tq.empty())
		{
			do
			{
				T.s = rand()%PicInfo.AllNodeNum;
			}
			while(Head[T.s] != -1);
		}
		else
		{
			T = tq.front();
			tq.pop();
			if(Head[T.s] != -1)
			{
				Nodenum++;
				continue;
			}
		}

		OutDegree = rand() % (PicInfo.MaxOutDegree - PicInfo.MinOutDegree + 1) + PicInfo.MinOutDegree; 
		while(OutDegree--)
		{
			do
			{
				T.e = rand() % PicInfo.AllNodeNum;
			}
			while(T.s == T.e);
			T.l = rand() % ELMax + 1;
			T.i = i++;
			setroad(T);
			vecTopo.push_back(T);
			if(Head[T.e] == -1)
			{
				TT = T;
				TT.s = T.e;
				tq.push(TT);
			}
		}
	}
	return 0;
}

void getmap(info PicInfo)
{
	int i,j,s,e,diss;
	vector<int> passroad;
	for(i = 0;i<PicInfo.AllNodeNum ;i++)
	{
		for(j = 0;j < PicInfo.AllNodeNum ; j++)
		{
			MapRoad[i][j].dlen=INF;
			MapRoad[i][j].PassRoad.clear();
		}
	}
	for(i = 0;i< PicInfo.AllNodeNum ; i++)
	{
		for(j = Head[i]; j != -1; j = road[j].next)
		{
			e = road[j].e;
			s = road[j].s;
			diss = MapRoad[s][e].dlen;
			if(deflag[j]==0 && road[j].l < diss)
			{
				MapRoad[s][e].dlen = road[j].l;
				MapRoad[s][e].PassRoad.clear();
				MapRoad[s][e].PassRoad.push_back(j);
			}
		}
	}
}

bool prinansroad(int s,int e,vector<int> &vecans)
{
	char buf[400];
	vecans.push_back(s);
	int i,j;
	for(i = 0;i < AnsRoad[s][e].PassRoad.size(); i++)
	{
		j = AnsRoad[s][e].PassRoad[i];
		printf("%d|",j);
		sprintf(buf,"%d|",j);
		fprintf(fans,buf);
		deflag[j] = 1;
		vecans.push_back(road[j].e);
	}
	cout<<endl;

	if(AnsRoad[s][e].dlen == INF)
	{
		printf("��·�Ѳ�ʧ�ܣ�������\n");
		return false;
	}

	printf("roadlen=%d\n",AnsRoad[s][e].dlen);
	sprintf(buf,"\nroadlen=%d\n",AnsRoad[s][e].dlen);
	fprintf(fans,buf);
	cout<<endl;
	return true;
}


void folyd(info PicInfo)
{  
    int cnt = PicInfo.AllNodeNum;
	vector<int> passb,passc;
	int a,b,c,sameflag;
	for(int k=0; k < cnt ;k++)  
    for(int i=0; i < cnt ;i++)  
    for(int j=0; j < cnt ;j++)
	{
        if(tmp[i][j].dlen > AnsRoad[i][k].dlen + MapRoad[k][j].dlen)  
        {
			passb = AnsRoad[i][k].PassRoad;
			passc = MapRoad[k][j].PassRoad;
			sameflag = 0;
			for( b = 0; b < passb.size() && !sameflag; b++)
			{
				for( c = 0; c < passc.size() && !sameflag; c++)
				{
					if(road[passb[b]].s == road[passc[c]].e)
					{
						sameflag = 1;
					}
				}
			}
			if(sameflag == 1)
				continue;
			passb.insert(passb.end(),passc.begin(),passc.end());		
			tmp[i][j].dlen = AnsRoad[i][k].dlen + MapRoad[k][j].dlen;			
			tmp[i][j].PassRoad = passb;
		}
	}
}  

void map_copy(info PicInfo)
{  
	int cnt = PicInfo.AllNodeNum;
    for(int i = 0;i < cnt; i++)  
    for(int j = 0;j < cnt; j++)
	{  
		AnsRoad[i][j].dlen = tmp[i][j].dlen;
		AnsRoad[i][j].PassRoad = tmp[i][j].PassRoad;
		tmp[i][j].dlen = INF;  
    }  
}  



//Ѱ��N���������·
void FindNRoad(info PicInfo)
{
	int n = PicInfo.DemandNodeNum + PicInfo.FirstRoadNodeNum;
	for(int i = 0;i < PicInfo.AllNodeNum; i++)
	{
		for(int j=0;j < PicInfo.AllNodeNum;j++)
		{
			if(i==j)
			{
				AnsRoad[i][j].dlen = 0;
			}
			else
			{
				AnsRoad[i][j].dlen = INF;
			}
			AnsRoad[i][j].PassRoad.clear();
			tmp[i][j].dlen = INF;
		}
	}

	float radio;
	while(n--)
	{
		radio = float(PicInfo.DemandNodeNum + PicInfo.FirstRoadNodeNum - n )/float(PicInfo.DemandNodeNum + PicInfo.FirstRoadNodeNum) * 100;
		printf("%.2f %% \r",radio);
		folyd(PicInfo);
		map_copy(PicInfo);
		fflush(stdout);
	}
}


//��ӡ�����topo��Ϣ
void print_vecTopo(vector<TopoLine> vecTopo,info PicInfo)
{
	FILE *ftopo;
	int i;
	char buf[200];
	for(i = 0;i < vecTopo.size(); i++)
	{
		printf("%d: %d->%d ,len=%d\n",vecTopo[i].i,vecTopo[i].s,vecTopo[i].e,vecTopo[i].l);
	}
	
	ftopo = fopen("..\\data\\topo.csv","w");
	for(i = 0;i < vecTopo.size();i++)
	{
		sprintf(buf,"%d,%d,%d,%d\n",vecTopo[i].i,vecTopo[i].s,vecTopo[i].e,vecTopo[i].l);
		fprintf(ftopo,buf);
	}
	fclose(ftopo);
}

void GraphBuild(info PicInfo)
{
	int s, e, i, j, len,pointn,ni,l,m,roadi;
	int EdgNum, NodeNum;
	int maxpointj,maxpointn;
	char mycmd,buf[400],debuf[400],BuildPath[100];
	vector<TopoLine> vecTopo;
	vector<int> vecans;
	bool buildf = true;
	memset(deflag,0,sizeof(deflag));
	
	
	fans = fopen("..\\data\\result.csv","w");
	fdemand = fopen("..\\data\\demand.csv", "w");
	

	//���������ͼ
	graph_build(PicInfo,vecTopo);
	//��������õ�ͼ
	print_vecTopo(vecTopo,PicInfo);

	
	//���һ��·
	getmap(PicInfo);
	printf("·��1������...\n");
	FindNRoad(PicInfo);
	buildf = prinansroad(PicInfo.Start,PicInfo.End,vecans);//��������Ѿ����ı�ȥ��

	//��ڶ���·
	if(buildf == true)
	{
		demandput(vecans,PicInfo);
		
		vecans.clear();
		PicInfo.FirstRoadNodeNum = PicInfo.SecondRoadNodeNum;
		getmap(PicInfo);
		printf("·��2������...\n");
		FindNRoad(PicInfo);
		buildf = prinansroad(PicInfo.Start,PicInfo.End,vecans);
		if(buildf == true)
		{
			demandput(vecans,PicInfo);
		}
	}

	fclose(fdemand);
	fclose(fans);


}

int main(int argc, char* argv[])
{
	struct info PicInfo;
	char mycmd;
	//������ͼ��Ϣ
	srand((unsigned int)time(0));
	information_input(PicInfo);
	GraphBuild(PicInfo);

	while(1)
	{	
		printf("·�����ɽ������˳�������q����������������r \n");
		scanf("%c", &mycmd);
		getchar();
		if(mycmd == 'q')
		{
			return 0;
		}
		else if(mycmd == 'r')
		{
			GraphBuild(PicInfo);
		}
	}
	return 0;
}



void demandput(vector<int> vecans,info PicInfo)
{

	int demandn,ansn,demandp;
	char buf[600],debuf[600];
	demandn = PicInfo.DemandNodeNum;

	ansn = vecans.size();
	set<int> demandset;
	vector<int> demandvec;
	while(demandn)
	{
		demandp = rand() % ansn;
		if(vecans[demandp] == PicInfo.Start || vecans[demandp] == PicInfo.End)
		{
			continue;
		}
    	if(demandset.find(demandp) == demandset.end())
		{
			demandset.insert(demandp);
			demandvec.push_back(vecans[demandp]);
			demandn--;
		}
	}


	sprintf(debuf,"0,%d,%d,",PicInfo.Start,PicInfo.End);
	for(int i=0;i<demandvec.size();i++)
	{
		if(i!=demandvec.size()-1)
			sprintf(buf, "%d|", demandvec[i]);
		else 
			sprintf(buf, "%d\n",demandvec[i]);
		strcat(debuf,buf);
	}
	fprintf(fdemand,debuf);

}