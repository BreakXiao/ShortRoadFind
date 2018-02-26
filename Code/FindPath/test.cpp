#include<stdio.h>
#include<iostream>
#include <queue>
#include<math.h>
#include <windows.h>
#include <WinBase.h>
#include <stdlib.h>
#include<string.h>
using namespace std;

#define MAX_LINE_LEN 5000  //��󳤶�
#define MAX_DEMAND_NUM 2000  //��󶥵���
#define MAX_EDGE_NUM 2003*20  //������
#define Road_M 400050  //��·������������ֵΪ400050��
#define City_M 2020    //��������2020
#define Need_M 130     //�ؾ���������Ϊ130
#define INF 9999999    //·���Ϊ20*5000=100000������INFΪ9999999�㹻
#define FINDTIME 6000 
struct  //��·�ṹ��
{
	int s,e,l;  //s:·����ʼ�ˣ�e:·���նˣ�l:·��
	int next,up;   //����������next:��һ��·��up:��һ��·
}road[Road_M];
 
//srotn����Dijkstra�㷨�����������ȶ��е��жϵĽṹ��
struct sortn
{
	int id,len,rnum,re_num,over_num;
};

//
struct min_road
{
	int len,next,re,up;
	struct p_road *ans_r;
};
int BackLevel = 10;
int k_count = 0;
int k_sum = 0;
int k_sum_n = 0;
int pass_city[Need_M],pass_city_rnum[Need_M];
struct min_road *need_road[Need_M][Need_M][70];
int need_head[Need_M];
int goto_road_num[Need_M][Need_M];
int select_road[Need_M][Need_M];
int past_time;
int city_over[City_M];
int cmp(struct sortn a,struct sortn b) //  a<b�ǵ�����a>b�ǵݼ�,����ǽṹ�壬���ܼ�&
{
	if(a.re_num<b.re_num)
		return 1;
	else if(a.re_num==b.re_num)
	{
		if(a.len<b.len)
			return 1;
		else return 0;
	}
	else return 0;
};     

int ans_road[Road_M],now_road[Road_M]; //�����·���飬��ǰ��·����
int ans_road_n; //�����·����
int Head[City_M],vis[City_M],dis[City_M][City_M],get_need[City_M],v[City_M];
//��֮����ͷ�ڵ㣬���ʱ�־   ��2����̾���     �� ����need������飬 ���ʱ�־ 
		
int min_len,need_n,need[City_M],need_juge[Need_M],Na_flag,road_num;
//·����Сֵ������ڵ��������ڵ��Ƿ�����־������ڵ����飻�޽��־

int start,end; //��㣬�յ�
int city_max; //���п������ֵ
long t1,t2,now_time; //����ʱ��
long dfs_start_time;
struct p_road  //��·ָ�룬����bfs�м�¼֮ǰ���߹��ĵ�·
{
	int data; //��·���
	struct p_road *up; //��һ�ε�·
};
int juge_road[Road_M];
int diff_num;
int city_need_city[City_M];
int ans_need[City_M],ans_i[City_M],city_best_eva1[City_M],city_need_len[City_M],city_vis[City_M];
int pre_need[City_M],pre_dis[City_M],v2[City_M];
int dis_need[City_M][City_M];
int ans_len,repeat_num[City_M];
int s_new;
int vis_flag[Need_M];
int out_num[City_M],in_num[City_M],value[City_M],save1[City_M],save2[City_M];
_int64 eva_pass[Need_M][Need_M];
int up_Head[City_M],need2[City_M];
int get_need2[City_M],need_juge2[Need_M],need_n2,juge_road1[Road_M];
int max_over;
int eva_len[Need_M][Need_M],eva_v[Need_M][Need_M],up_need_head[Need_M];
int knum;
FILE *flog;
FILE *fput;
void log_print(char *str)
{
	//printf("%s",str);
	fprintf(flog,str);
}

//��s�ؾ���Ϊ��㣬��������ؾ�������·
void get_need_road1(int s,int f)
{
	int i, minn, select, j, e, city_road[City_M], r,city, r_num, re_max;
	struct p_road *p, *y;
	for(i = 0; i < city_max; i++)   //DJ֮ǰ�ĳ�ʼ��
	{
		dis[s][i] = INF;      //������ظ������������
		repeat_num[i] = INF; 
		if(city_vis[i] == 0)  //����õ�֮ǰû�߹�����v��0��������1��
			v[i] = 0;
		else v[i] = 1;
	}
	//s��s�ľ���Ϊ0���ظ�ֵҲΪ0
	dis[s][s] = 0;  
	repeat_num[s] = 0;
	//
	for(i = 0;i < city_max; i++)
	{
		minn = INF;
		re_max = INF;
		for(j = 0;j < city_max; j++)
		{
			if(v[j] == 0 && repeat_num[j] < re_max)    //ѡ���ظ�ֵ���ٵĽڵ㣬�ظ�ֵ�����ѡ·����̵�
			{
				minn = dis[s][j];
				re_max = repeat_num[j];
				select = j;
			}
			else if(v[j] == 0 && repeat_num[j] == re_max && dis[s][j] < minn)
			{
				minn = dis[s][j];
				re_max = repeat_num[j];
				select = j;
			}
		}

		if(minn == INF)
			break ;  //����Ҳ���min�ˣ�˵���Ѿ�û��·���ߣ������˳�
	
		v[select] = 1;   //��ǣ��ѱ�����
	
		//������ѡ�еĽڵ��Ǳؾ��㣬�Ҳ���s�㣬����С���·������
		//��·��ָ�ؾ���͸�s��֮������· ��Ϊһ���ߡ�
		if(need[select] == 1 && (select != s))
		{		
			//��s����㣬�ұ�ѡ�е�Ϊ�յ㣬�ұؾ������2�Ļ���������·������
			//��Ϊ���·û���塣����Ҫ�����м����ܵ��յ㡣
			if(s == start && select == end && need_n != 2)
				continue;
			//�ߵ����Ҳû���壬������
			if(select == start)
				continue;
			//û��Ҫ��·�ıؾ��㣬�򲻼�·
			if((vis_flag[get_need[select]]) == 0) 
				continue;
			else 
				f--;
			//s��select֮��������r_num��·������Ҫ�������ӵ�r_num+1��·��
			//need_road[s][select][r_num]����һ��·��
			//get_need[x]: ��x���Ƴ��õ��ڱؾ��㼯�е���š�
			//goto_road_num[s][select]:��s��x֮���Ѿ��ҵ��˶�������ѡ·��
			r_num=goto_road_num[ get_need[s] ][ get_need[select] ];

			if(r_num <= 50) //s��select֮���趨���50��·
			{
				if(need_road[get_need[s]][get_need[select]][r_num] != NULL)  //���еĻ����ͷ��ڴ�
				{
					delete need_road[get_need[s]][get_need[select]][r_num];
					need_road[get_need[s]][get_need[select]][r_num] = NULL;
				}
				//�½�min_road��min_road������·������Ϣ
				need_road[get_need[s]][get_need[select]][r_num] = new struct min_road; 
				//�½�·��ans_r����������·�����˾�����
				need_road[get_need[s]][get_need[select]][r_num]->ans_r = new struct p_road; 
				 //s��select֮���·������+1
				(goto_road_num[get_need[s]][get_need[select]])++;
			}
			else
			{
				r_num = 50; //����Ѿ�������ʮ���������½���ֱ�Ӹ�������Ķ���
			}
			//������·�ĳ��Ⱥ��ظ�����ֵ
			need_road[get_need[s]][get_need[select]][r_num]->len = dis[s][select];
			need_road[get_need[s]][get_need[select]][r_num]->re = repeat_num[select];

			//���r_numΪ0��˵���ǵ�һ�μ�·����Ҫ����Ϣ���ӽ�head[s]���ڽӱ���
			if(r_num == 0) 
			{
				need_road[get_need[s]][get_need[select]][0]->next = need_head[get_need[s]];
				need_head[get_need[s]] = get_need[select];
				need_road[get_need[s]][get_need[select]][0]->up = up_need_head[get_need[select]];
				up_need_head[get_need[select]] = get_need[s];
			}

			//�����·�м侭����С�ڵ�
			r = city_road[select];
			p = need_road[get_need[s]][get_need[select]][r_num]->ans_r;
			p->up = NULL;
			city = select;
			while(city != s)
			{
				y = new struct p_road;
				y->data = r;
				y->up = p->up;
				p->up = y;
				city = road[r].s;
				r = city_road[city];
			}
			//���Ҫ����·�ĵ��Ѿ����������ˣ����˳�
			if(f == 0)
				return ;
			continue;
		}
		//�ܾ����¾����ؾ����·��
		if(select==start&&s!=start)
			continue;
		if(select==end)
			continue;

		//DJ�ĸ��²���
		for(j = Head[select];j != -1;j = road[j].next)
		{
			e = road[j].e;
			if(v[e] == 1)
				continue;
			//���ظ�ֵ�ִ��ҳ����ֳ�ʱ����ִ�и��²���
			if( repeat_num[select] + juge_road[j] + juge_road1[j] < repeat_num[e] )
			{
				city_road[e]=j;
				dis[s][e]=dis[s][select]+road[j].l;
				repeat_num[e]=repeat_num[select]+juge_road[j]+juge_road1[j];
			}
			else if(repeat_num[select] + juge_road[j] + juge_road1[j] == repeat_num[e])
			{
				if(dis[s][select] + road[j].l < dis[s][e])
				{
					city_road[e]=j;
					dis[s][e]=dis[s][select]+road[j].l;
					repeat_num[e]=repeat_num[select]+juge_road[j]+juge_road1[j];					
				}
			}
		}
	}
}


void dfs(int s,int road_n,int len,int get_need_n)
{
	int i,e,l,j,select_next[Need_M],k,pass_n,end_len,eva_sum=0;
	int f1,f2,f3,t3,t4,t5,t6,select,big_len=INF,kk,eva_flag;
	int select_num=0,over_num; 
	struct p_road *p;
	struct sortn sort_need[Need_M];	
	char buf[100];
	if(min_len != INF) //����Ѿ��ѵ�·����һֱ���ϻ���
	{
		sprintf(buf,"��ǰ�ڵ�%d�㣬���ϻ���\n",get_need_n);	
		fprintf(flog,buf);
		return ;
	}

	//t2��ʱ
	t2 = GetTickCount();

	//����ʱ���˳�
	if(ans_len!=INF && t2-dfs_start_time > FINDTIME )
	{
		sprintf(buf,"��ʱ�˳���\n");	
		fprintf(flog,buf);
		min_len=INF-1;
		return ;
	}

	sprintf(buf,"��ǰΪ��%d�㣬�����ڱؾ���%d\n",get_need_n,need_juge[s]);	
	fprintf(flog,buf);
	/*�ҵ��յ��ұؾ��㶼����Ҫ��ʱ�Ĵ���*/
	if(need_juge[s] == end)  //�ҵ��յ�ʱ
	{
		if(get_need_n == need_n) //�ж�������Ƿ��Ѿ���
		{
			if(len < min_len) //�жϵ�ǰ·���Ƿ����֪���·С
			{
				min_len = len;  //�������·��	
				road_num = 0;   //�м�·������
				over_num = 0;   //�ر�����
				//�����ر���
				for(i = 0;i < road_n - 1; i++)
				{
					//ͨ��upָ����е��ơ�
					p = need_road[pass_city[i]][pass_city[i+1]][pass_city_rnum[pass_city[i]]]->ans_r->up;
					while(p != NULL)
					{
						if(diff_num == 0 && juge_road[p->data] == 1)
							over_num++;
						else if(diff_num == 1 && juge_road1[p->data] == 1)
							over_num++;
						p = p->up;
					}
				}
				//����ر�����֮ǰ�࣬�򲻸���
				if(over_num > max_over)
					return ;
				else max_over = over_num;
				//���·���ȱ�֮ǰС���򲻸���
				if(min_len >= ans_len)  
					return ;
				else ans_len = min_len;
				cout<<"len="<<len;
				t2=GetTickCount(); //��ʱ1
				cout<<"  time="<<t2-t1<<endl;
				sprintf(buf,"�ȵ�ǰ���·�ţ����£�������·Ϊ��\n");	
				fprintf(flog,buf);
				//���������ıߴ���ans_road����
				for(i = 0;i < road_n - 1;i++)
				{
					p = need_road[pass_city[i]][pass_city[i+1]][pass_city_rnum[pass_city[i]]]->ans_r->up;
					while(p != NULL)
					{
						sprintf(buf,"%d|",p->data);	
						fprintf(flog,buf);

						ans_road[road_num] = p->data;
						p = p->up;
						road_num++;
					}
				}
				ans_road_n = road_num;
				sprintf(buf,"\n");	
				fprintf(flog,buf);
				cout<<endl;
			}
		}
		return ;
	}
	
	/*********�Կ���Ҫ�ߵ�·��������ѡ�����ŵ�һ��·*********/
    k = kk = 0;
	select = 0;
	memset(vis_flag, 0, sizeof(vis_flag));
	for(i = need_head[s];i != -1;i = need_road[s][i][0]->next)
	{
		vis_flag[i] = 0;
		if(vis[i] == 1)
		{		
			continue;
		}
		for(j = 0; j < goto_road_num[s][i]; j++)
		{
			vis_flag[i] = 0;
			sort_need[k].over_num = 0;
			p = need_road[s][i][j]->ans_r->up;
			while(p != NULL)
			{
				if(city_vis[road[p->data].s] == 1)
				{	
					vis_flag[i] = 1;
					break;
				}
				p = p->up;
			}
			if(vis_flag[i] == 0)
			{
				break;
			}

		}
		sort_need[k].rnum=j;
		k++;
		if(vis_flag[i]==1)
		{
			select=1;
			kk++;
		}
	}

	if(select==1) 
	{
		//����Ҫ���µĵ����k/2ʱ���������·�����¼��㡣
		if(kk>=k/2)
		{
			get_need_road1(need_juge[s],kk);
			k_count++;
		}
	}
	k=0;
	//���δ���sort_need���飬׼����������
	for(i = need_head[s];i != -1;i = need_road[s][i][0]->next)
	{

		if(vis[i] == 1)
			continue;
		
		if(need_road[s][i][sort_need[k].rnum] == NULL)
		{
			sort_need[k].len=INF;
			sort_need[k].re_num=INF;
			k++;
			continue;
		}
		sort_need[k].len = need_road[s][i][sort_need[k].rnum]->len + eva_len[need_n-1-get_need_n][i];
		sort_need[k].re_num = need_road[s][i][sort_need[k].rnum]->re;
		sort_need[k].id = i;
		k++;
	}
	//�����ظ�ֵС�����ȣ��������lenС������
	if(k != 0)
	{
		sort(sort_need,sort_need+k,cmp);
	}

	/*********��ʼ�������򣬽���dfs����*********/
	for(i = 0;i < k && min_len == INF; i++)
	{
		//���dj֮���ǲ���������·������
		if(sort_need[i].len==INF)
		{
			continue;
		}
		e = sort_need[i].id;
		
		//�����һ����Ϊ�յ��ұؾ�������δ����������
		if(e == get_need[end] && (get_need_n+1) != need_n)
		{
			continue;
		}
		//�������
		if(e == 0)
			continue;
		if(vis[e] == 1)  //�����һ�������Ѿ����ѹ���������һ��·
		{
			continue;
		}	
		//������һ��dfs��׼����������ֵ�����ʽڵ�vis��1
		l = need_road[s][e][sort_need[i].rnum]->len;
		p = need_road[s][e][sort_need[i].rnum]->ans_r->up;
		pass_n = 1;
		while(p != NULL)
		{
			city_vis[road[p->data].s] = 1;
			p = p->up;
		}
		vis[e] = 1; //����һ�����б�Ϊ���߹�
		pass_city[road_n] = e; //dfs�����ĳ���
		pass_city_rnum[s] = sort_need[i].rnum;
		dfs(e,road_n+1,len+l,get_need_n+1); //������	
		//����ҵ�·�ˣ������ݵ�����ˣ���ʱ�䣬������ʱ�䷶Χ�ڣ����ٻ��ݣ�����һ��·
		if(get_need_n <= BackLevel && min_len != INF)
		{
			t2 = GetTickCount();
			if(diff_num == 1 && t2-dfs_start_time < FINDTIME )
			{
				min_len = INF;
				k_sum += k_count;
				k_sum_n++;
				k_count = 0;
			}
			else if(diff_num == 0 && t2-dfs_start_time < FINDTIME )
			{
				min_len = INF;
				k_sum += k_count;
				k_sum_n++;
				k_count = 0;
			}
		}

		//ȡ��֮ǰ�ı�ǣ�����һ��
		vis[e]=0;//ȡ�����
		p=need_road[s][e][sort_need[i].rnum]->ans_r->up;
		while(p != NULL)
		{
			city_vis[road[p->data].s]=0;
			p=p->up;
		}
	}
}

void dfs_init()
{
	struct p_road *p;
	int i,j;
	long ts,te;
	char buf[1000];
	s_new=-1;
	for(i=0;i<City_M;i++)
	{
		city_over[i]=0;
	}
	for(i=0;i<need_n+3;i++)
	{
		need_head[i]=-1;
		up_need_head[i]=-1;
	}

	memset(goto_road_num,0,sizeof(goto_road_num));
	for(i=0;i<need_n;i++)
	{
		memset(city_vis,0,sizeof(city_vis));
		for(j=0;j<need_n;j++)
		{
			vis_flag[j]=1;
			eva_len[i][j]=0;
		}
		get_need_road1(need_juge[i],-1);//��need_juge[i]����㵽�����ؾ�������·	
		
		
		for(j = need_head[i];j != -1;j = need_road[i][j][0]->next)
		{		
			sprintf(buf,"%d->%d, len=%d, covernum=%d\n",need_juge[i],need_juge[j],need_road[i][j][0]->len,need_road[i][j][0]->re);	
			fprintf(flog,buf);

			p = need_road[i][j][0]->ans_r->up;
			sprintf(buf,"passroad:");
			fprintf(flog, buf);
			while(p!=NULL)
			{
				sprintf(buf,"|%d",p->data);
				fprintf(flog,buf);
				p=p->up;
			}
			sprintf(buf,"\n\n");
			fprintf(flog,buf);
		}
	}	
	min_len=INF;
	ans_len=INF;
	memset(vis,0,sizeof(vis));
	vis[0]=1;
	pass_city[0]=0;
	memset(city_vis,0,sizeof(city_vis));
}


void write_road(int f)
{
	int i;
	char buf[2048];
	if(ans_len==INF)
	{
		printf("�����·�޽�\n");
		return ;
	}	
	printf("���·�н⣬���·��Ϊ\n");
	printf("%d\n",ans_len);
	printf("������·��Ϊ�� \n");

	for(i=0;i<ans_road_n;i++)
	{
		printf("%d|",ans_road[i]);
		sprintf(buf,"%d|",ans_road[i]);
		fprintf(fput,buf);
		
		if(f==1)
			juge_road[ans_road[i]]=1;
		if(f==2)
			if(juge_road[ans_road[i]]==0)
				diff_num++;
	}
	cout<<endl;
	sprintf(buf,"\n");
	fprintf(fput,buf);


	printf("��������˳��Ϊ�� ");
	printf("%d",start);
	sprintf(buf,"%d",start);
	fprintf(fput,buf);
	for(i = 0;i<ans_road_n;i++)
	{
		printf("->%d",road[ans_road[i]].e);
		sprintf(buf,"->%d",road[ans_road[i]].e);
		fprintf(fput,buf);
	}
	sprintf(buf,"\n");
	fprintf(fput,buf);
	cout<<endl;
}

int read_file(char ** const buff, const unsigned int spec, const char * const filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Fail to open file %s, %s.\n", filename, strerror(errno));
        return 0;
    }

    char line[MAX_LINE_LEN + 2];
    unsigned int cnt = 0;
    while ((cnt < spec) && !feof(fp))
    {
        line[0] = 0;
        if (fgets(line, MAX_LINE_LEN + 2, fp) == NULL)  continue;
        if (line[0] == 0)   continue;
        buff[cnt] = (char *)malloc(MAX_LINE_LEN + 2);
        strncpy(buff[cnt], line, MAX_LINE_LEN + 2 - 1);
        buff[cnt][MAX_LINE_LEN + 1] = 0;
        cnt++;
    }
    fclose(fp);
   // printf("There are %d lines in file %s.\n", cnt, filename);

 return cnt;
}


int get_cntt(int cnt,int cntt,char **const str)
{
	while(1)
	{
		if(str[cnt][cntt]<'0'||str[cnt][cntt]>'9')
		{
			cntt=cntt+1;
			break;
		}	
		cntt++;
	}
	return cntt;
}


void data_init()
{
	int i;
	for(i=0;i<Road_M;i++)
	{
		juge_road[i]=0;
		juge_road1[i]=0;
	}
	for(i=0;i<City_M;i++)
	{
		vis[i]=need[i]=0;
		Head[i]=-1;
		up_Head[i]=-1;
		out_num[i]=in_num[i]=0;
	}
}

void read_road(int i,int s,int e,int l)
{
	int RE_flag,j;
	if(s+2>city_max)
			city_max=s+2;
	if(e+2>city_max)
		city_max=e+2;
	RE_flag=0;
	if(RE_flag==0)
	{
		road[i].s=s;
		road[i].e=e;
		road[i].l=l;
		road[i].next=Head[s];
		Head[s]=i;
		road[i].up=up_Head[e];
		up_Head[e]=i;
		out_num[s]++;
		in_num[e]++;
	}
}


void road_deel1(int p)
{
	int i,j;
	char buf[200];
	if(p == start||p == end)
		return ;
	if(out_num[p] <= 1)
	{
		for(i = Head[p];i != -1;i = road[i].next)
		{
			if(road[i].e == end||road[i].e == start)
				continue;
			juge_road1[i]++;
			sprintf(buf,"��%d ���ظ�ֵ��1 \n",i);
			log_print(buf);
			road_deel1(road[i].e);
		}
	}
}


void road_deel2(int p)
{
	int i,j;
	char buf[200];
	if(p==start||p==end)
		return ;
	if(out_num[p]<=1)
	{
		for(j=up_Head[p];j!=-1;j=road[j].up)
		{
			if(road[j].s==end||road[j].s==start)
			{
				continue;
			}
			juge_road1[j]++;
			sprintf(buf,"��%d ���ظ�ֵ��1 \n",j);
			log_print(buf);
			road_deel2(road[j].s);
		}
	}
}


int check[1000];

//��ȡ�����ļ�
void read_information(char ** const topo,int edg_num,char ** const demand)
{
	int i,s,e,l,cntt,p,j,RE_flag,rr,cnt=0,ans_sum;
	char str;
	char buf[4096];
	int check_num=0;
	memset(check,0,sizeof(check));

	fput = fopen("..\\data\\myresult.csv","w");
	data_init();
	ans_sum = 0;
	city_max = -1;
	t1 = GetTickCount(); //��ʱ1
	while(1)
	{
		cntt = 0;
		i = atoi(topo[cnt]+cntt);
		cntt = get_cntt(cnt,cntt,topo);
		s = atoi(topo[cnt]+cntt);
		cntt = get_cntt(cnt,cntt,topo);
		e = atoi(topo[cnt]+cntt);
		cntt = get_cntt(cnt,cntt,topo);
		l = atoi(topo[cnt]+cntt);
		read_road(i, s, e, l);
		if(check[s] == 0)
		{
			check[s] = 1;
			check_num++;
		}
		cnt++;
		if(cnt == edg_num)
			break;
	}





	printf("node num=%d\n",check_num);
	cntt = 0;
	cnt = 0;
	
	cntt = get_cntt(cnt,cntt,demand);
	start = atoi(demand[cnt]+cntt);
	cntt = get_cntt(cnt,cntt,demand);
	end = atoi(demand[cnt]+cntt);

	printf("start1=%d,end1=%d\n",start,end);
	get_need[start] = 0;
	need_juge[0] = start;
	get_need[end] = 1;
	need_juge[1] = end;
	need[end] = need[start] = 1;
	need_n = 2;
	printf("neccsety = ");
	while(1)
	{
		cntt=get_cntt(cnt,cntt,demand);
		if(demand[cnt][cntt-1] == '\n'||demand[cnt][cntt] == 'N')
			break;
		p = atoi(demand[cnt]+cntt);
		need[p] = 1;
		need_juge[need_n] = p;
		get_need[p] = need_n;
		need_n++;
		printf("%d ",p);
	}
	cout<<endl;
	printf("need_n=%d\n",need_n);

	
	//int radio = noden;
	if(check_num < 50)
	{
		BackLevel = 10;
	}
	else if(check_num <100)
		BackLevel = 8;
	else if(check_num < 200)
		BackLevel = 6;
	else if(check_num <300)
		BackLevel =4;
	else if(check_num < 500)
		BackLevel = 3;
	else BackLevel = 2;








	memset(need2,0,sizeof(need2));
	cntt=0;
	cnt=1;
	cntt=get_cntt(cnt,cntt,demand);
	start = atoi(demand[cnt]+cntt);
	cntt = get_cntt(cnt,cntt,demand);
	end = atoi(demand[cnt]+cntt);
	printf("start2=%d,end2=%d\n",start,end);
	get_need2[start] = 0;
	need_juge2[0] = start;
	get_need2[end] = 1;
	need_juge2[1] = end;
	need2[end] = need2[start] = 1;
	need_n2 = 2;
	printf("neccsety= ");
	while(1)
	{
		cntt=get_cntt(cnt,cntt,demand);
		if(demand[cnt][cntt-1] == '\n'||demand[cnt][cntt-1] == '\0' || demand[cnt][cntt] == 'N')
			break;
		p=atoi(demand[cnt] + cntt);
		need2[p] = 1;
		need_juge2[need_n2] = p;
		get_need2[p] = need_n2;
		if(out_num[p] <= 1)
		{
			sprintf(buf,"\n��ѡ·�з��ֳ���Ϊ1�ĵ㣺%d\n",p);
			log_print(buf);
			for(j = Head[p];j != -1;j = road[j].next)
			{
				if(road[j].e == end||road[j].e == start)
				{
					continue;
				}
				juge_road1[j]++;

				sprintf(buf,"��%d ���ظ�ֵ��1 \n",j);
				log_print(buf);
				
				road_deel1(road[j].e);
			}	
			for(j = up_Head[p];j != -1;j = road[j].up)
			{
			//	sprintf(buf,"��%d��jugeȨֵ��1",);
				if(road[j].s==end||road[j].s==start)
					continue;
				juge_road1[j]++;
				sprintf(buf,"��%d ���ظ�ֵ��1 \n",j);
				log_print(buf);
				road_deel2(road[j].s);
			}
		}
		need_n2++;
		printf("%d ",p);
	}
	printf("need_n2=%d\n",need_n2);
	cout<<endl;


	printf("�ѵ�һ��·\n");
	dfs_start_time = GetTickCount();
	diff_num = 1;
	max_over = INF;
	dfs_init();	

	k_count = 0;
	dfs(0,1,0,1);
	write_road(1);
	
	sprintf(buf,"len1 = %d\n", ans_len);
	fprintf(fput,buf);

	ans_sum += ans_len;

	printf("\n�ѵڶ���·��\n");
	for(i = 0;i < City_M; i++)
	{
		need[i] = need2[i];
		get_need[i] = get_need2[i];
	}

	for(i=0;i<need_n2;i++)
	{
		need_juge[i]=need_juge2[i];
	}
	need_n=need_n2;
	dfs_start_time = GetTickCount();
	diff_num=0;
	max_over=INF;
	dfs_init();
	k_count = 0;
	dfs(0,1,0,1);
	write_road(2);
	
	sprintf(buf,"len2 = %d\n", ans_len);
	fprintf(fput,buf);

	ans_sum += ans_len;
	t2=GetTickCount(); //��ʱ1
	printf("road=%d,sameroad=%d,ans_sum=%d,time=%d\n",ans_road_n,ans_road_n-diff_num,ans_sum,t2-t1);
	sprintf(buf,"road=%d,sameroad=%d,ans_sum=%d,time=%d\n",ans_road_n,ans_road_n-diff_num,ans_sum,t2-t1);
	fprintf(fput,buf);
	fclose(fput);
}

int main(int argc, char* argv[])
{
    printf("Begin");
	flog = fopen("..\\log\\testlog.log","w");
    char *topo[MAX_EDGE_NUM];
    int edge_num;
    char *demand[MAX_DEMAND_NUM];
    int demand_num;
	char toponame[80],demandname[80];
	strcpy(toponame,argv[1]);
	strcpy(demandname,argv[2]);
    edge_num = read_file(topo, MAX_EDGE_NUM, toponame);//topo�ļ���
    if (edge_num == 0)
    {
        printf("Please input valid topo file.\n");
        return -1;
    }
    demand_num = read_file(demand, MAX_DEMAND_NUM, demandname); //demand�ļ�������ʽ�͸���Ҫ��һ��    
	read_information(topo,edge_num,demand);
    printf("End");
	fclose(flog);
	while(1);
	return 0;
}

