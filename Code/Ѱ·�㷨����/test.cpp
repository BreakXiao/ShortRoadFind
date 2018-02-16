#include<stdio.h>
#include<iostream>
#include <queue>
#include<math.h>
#include <windows.h>
#include <WinBase.h>
#include <stdlib.h>
#include<string.h>
using namespace std;

#define MAX_LINE_LEN 5000  //最大长度
#define MAX_DEMAND_NUM 2000  //最大顶点数
#define MAX_EDGE_NUM 2003*20  //最大边数
#define Road_M 400050  //道路总量，设个最大值为400050条
#define City_M 2020    //城市总量2020
#define Need_M 130     //必经城市总量为130
#define INF 9999999    //路长最长为20*5000=100000，故设INF为9999999足够
#define FINDTIME 6000 
struct  //道路结构体
{
	int s,e,l;  //s:路的起始端，e:路的终端，l:路长
	int next,up;   //链接向量，next:下一条路，up:上一条路
}road[Road_M];
 
//srotn：在Dijkstra算法中用来做优先队列的判断的结构体
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
int cmp(struct sortn a,struct sortn b) //  a<b是递增，a>b是递减,如果是结构体，不能加&
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

int ans_road[Road_M],now_road[Road_M]; //输出道路数组，当前道路数组
int ans_road_n; //输出道路总量
int Head[City_M],vis[City_M],dis[City_M][City_M],get_need[City_M],v[City_M];
//点之出度头节点，访问标志   ，2点最短距离     ， 反推need标号数组， 访问标志 
		
int min_len,need_n,need[City_M],need_juge[Need_M],Na_flag,road_num;
//路程最小值；所需节点总量；节点是否必需标志；必须节点数组；无解标志

int start,end; //起点，终点
int city_max; //城市可能最大值
long t1,t2,now_time; //测试时间
long dfs_start_time;
struct p_road  //道路指针，用于bfs中记录之前所走过的道路
{
	int data; //道路标号
	struct p_road *up; //上一次道路
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

//以s必经点为起点，求出到各必经点的最短路
void get_need_road1(int s,int f)
{
	int i, minn, select, j, e, city_road[City_M], r,city, r_num, re_max;
	struct p_road *p, *y;
	for(i = 0; i < city_max; i++)   //DJ之前的初始化
	{
		dis[s][i] = INF;      //距离和重复数都先置最大
		repeat_num[i] = INF; 
		if(city_vis[i] == 0)  //如果该点之前没走过，则v置0，否则置1。
			v[i] = 0;
		else v[i] = 1;
	}
	//s到s的距离为0，重复值也为0
	dis[s][s] = 0;  
	repeat_num[s] = 0;
	//
	for(i = 0;i < city_max; i++)
	{
		minn = INF;
		re_max = INF;
		for(j = 0;j < city_max; j++)
		{
			if(v[j] == 0 && repeat_num[j] < re_max)    //选出重复值最少的节点，重复值相等则选路程最短的
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
			break ;  //如果找不到min了，说明已经没有路可走，提早退出
	
		v[select] = 1;   //标记，已被访问
	
		//若现在选中的节点是必经点，且不是s点，则进行“加路”操作
		//加路：指必经点和该s点之间的最短路 简化为一条边。
		if(need[select] == 1 && (select != s))
		{		
			//若s是起点，且被选中点为终点，且必经点大于2的话，则这条路不生成
			//因为这个路没意义。必须要经过中间点才能到终点。
			if(s == start && select == end && need_n != 2)
				continue;
			//走到起点也没意义，不生成
			if(select == start)
				continue;
			//没必要加路的必经点，则不加路
			if((vis_flag[get_need[select]]) == 0) 
				continue;
			else 
				f--;
			//s到select之间现在有r_num条路，我们要给他增加第r_num+1条路。
			//need_road[s][select][r_num]决定一条路。
			//get_need[x]: 从x点推出该点在必经点集中的序号。
			//goto_road_num[s][select]:从s到x之间已经找到了多少条备选路。
			r_num=goto_road_num[ get_need[s] ][ get_need[select] ];

			if(r_num <= 50) //s到select之间设定最多50条路
			{
				if(need_road[get_need[s]][get_need[select]][r_num] != NULL)  //已有的话就释放内存
				{
					delete need_road[get_need[s]][get_need[select]][r_num];
					need_road[get_need[s]][get_need[select]][r_num] = NULL;
				}
				//新建min_road，min_road包含该路所有信息
				need_road[get_need[s]][get_need[select]][r_num] = new struct min_road; 
				//新建路，ans_r包含着这条路的拓扑经历。
				need_road[get_need[s]][get_need[select]][r_num]->ans_r = new struct p_road; 
				 //s到select之间的路的数量+1
				(goto_road_num[get_need[s]][get_need[select]])++;
			}
			else
			{
				r_num = 50; //如果已经到达五十条，则不再新建，直接更改里面的东西
			}
			//给这条路的长度和重复数赋值
			need_road[get_need[s]][get_need[select]][r_num]->len = dis[s][select];
			need_road[get_need[s]][get_need[select]][r_num]->re = repeat_num[select];

			//如果r_num为0，说明是第一次加路，需要将信息增加进head[s]的邻接表中
			if(r_num == 0) 
			{
				need_road[get_need[s]][get_need[select]][0]->next = need_head[get_need[s]];
				need_head[get_need[s]] = get_need[select];
				need_road[get_need[s]][get_need[select]][0]->up = up_need_head[get_need[select]];
				up_need_head[get_need[select]] = get_need[s];
			}

			//存入此路中间经过的小节点
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
			//如果要增加路的点已经都增加完了，则退出
			if(f == 0)
				return ;
			continue;
		}
		//拒绝更新经过必经点的路径
		if(select==start&&s!=start)
			continue;
		if(select==end)
			continue;

		//DJ的更新操作
		for(j = Head[select];j != -1;j = road[j].next)
		{
			e = road[j].e;
			if(v[e] == 1)
				continue;
			//当重复值又大，且长度又长时，不执行更新操作
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
	if(min_len != INF) //如果已经搜到路，则一直向上回溯
	{
		sprintf(buf,"当前在第%d层，向上回溯\n",get_need_n);	
		fprintf(flog,buf);
		return ;
	}

	//t2计时
	t2 = GetTickCount();

	//若超时则退出
	if(ans_len!=INF && t2-dfs_start_time > FINDTIME )
	{
		sprintf(buf,"超时退出！\n");	
		fprintf(flog,buf);
		min_len=INF-1;
		return ;
	}

	sprintf(buf,"当前为第%d层，正处于必经点%d\n",get_need_n,need_juge[s]);	
	fprintf(flog,buf);
	/*找到终点且必经点都满足要求时的处理*/
	if(need_juge[s] == end)  //找到终点时
	{
		if(get_need_n == need_n) //判断所需点是否都已经过
		{
			if(len < min_len) //判断当前路程是否比已知最短路小
			{
				min_len = len;  //更新最短路程	
				road_num = 0;   //中间路的数量
				over_num = 0;   //重边数量
				//计算重边数
				for(i = 0;i < road_n - 1; i++)
				{
					//通过up指针进行倒推。
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
				//如果重边数比之前多，则不更新
				if(over_num > max_over)
					return ;
				else max_over = over_num;
				//如果路长度比之前小，则不更新
				if(min_len >= ans_len)  
					return ;
				else ans_len = min_len;
				cout<<"len="<<len;
				t2=GetTickCount(); //计时1
				cout<<"  time="<<t2-t1<<endl;
				sprintf(buf,"比当前最短路优，更新！所经过路为：\n");	
				fprintf(flog,buf);
				//将所经过的边存入ans_road数组
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
	
	/*********对可能要走的路进行排序，选择最优的一条路*********/
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
		//当需要更新的点大于k/2时，进行最短路的重新计算。
		if(kk>=k/2)
		{
			get_need_road1(need_juge[s],kk);
			k_count++;
		}
	}
	k=0;
	//依次存入sort_need数组，准备进行排序
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
	//排序，重复值小的优先，其次再是len小的优先
	if(k != 0)
	{
		sort(sort_need,sort_need+k,cmp);
	}

	/*********开始按照排序，进行dfs操作*********/
	for(i = 0;i < k && min_len == INF; i++)
	{
		//如果dj之后还是不存在这条路，跳过
		if(sort_need[i].len==INF)
		{
			continue;
		}
		e = sort_need[i].id;
		
		//如果下一个点为终点且必经点数量未满，则跳过
		if(e == get_need[end] && (get_need_n+1) != need_n)
		{
			continue;
		}
		//跳过起点
		if(e == 0)
			continue;
		if(vis[e] == 1)  //如果下一个城市已经被搜过，则搜下一条路
		{
			continue;
		}	
		//进入下一层dfs的准备操作，赋值，访问节点vis置1
		l = need_road[s][e][sort_need[i].rnum]->len;
		p = need_road[s][e][sort_need[i].rnum]->ans_r->up;
		pass_n = 1;
		while(p != NULL)
		{
			city_vis[road[p->data].s] = 1;
			p = p->up;
		}
		vis[e] = 1; //把下一个城市标为已走过
		pass_city[road_n] = e; //dfs经过的城市
		pass_city_rnum[s] = sort_need[i].rnum;
		dfs(e,road_n+1,len+l,get_need_n+1); //往下搜	
		//如果找到路了，并回溯到最顶层了，则看时间，若还在时间范围内，不再回溯，找下一条路
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

		//取消之前的标记，找下一条
		vis[e]=0;//取消标记
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
		get_need_road1(need_juge[i],-1);//求need_juge[i]这个点到其他必经点的最短路	
		
		
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
		printf("此最短路无解\n");
		return ;
	}	
	printf("最短路有解，最短路程为\n");
	printf("%d\n",ans_len);
	printf("所经过路径为： \n");

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


	printf("所经过点顺序为： ");
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
			sprintf(buf,"边%d 的重复值加1 \n",i);
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
			sprintf(buf,"边%d 的重复值加1 \n",j);
			log_print(buf);
			road_deel2(road[j].s);
		}
	}
}


int check[1000];

//读取输入文件
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
	t1 = GetTickCount(); //计时1
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
			sprintf(buf,"\n备选路中发现出度为1的点：%d\n",p);
			log_print(buf);
			for(j = Head[p];j != -1;j = road[j].next)
			{
				if(road[j].e == end||road[j].e == start)
				{
					continue;
				}
				juge_road1[j]++;

				sprintf(buf,"边%d 的重复值加1 \n",j);
				log_print(buf);
				
				road_deel1(road[j].e);
			}	
			for(j = up_Head[p];j != -1;j = road[j].up)
			{
			//	sprintf(buf,"边%d的juge权值加1",);
				if(road[j].s==end||road[j].s==start)
					continue;
				juge_road1[j]++;
				sprintf(buf,"边%d 的重复值加1 \n",j);
				log_print(buf);
				road_deel2(road[j].s);
			}
		}
		need_n2++;
		printf("%d ",p);
	}
	printf("need_n2=%d\n",need_n2);
	cout<<endl;


	printf("搜第一条路\n");
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

	printf("\n搜第二条路！\n");
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
	t2=GetTickCount(); //计时1
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
    edge_num = read_file(topo, MAX_EDGE_NUM, toponame);//topo文件名
    if (edge_num == 0)
    {
        printf("Please input valid topo file.\n");
        return -1;
    }
    demand_num = read_file(demand, MAX_DEMAND_NUM, demandname); //demand文件名，格式和复赛要求一样    
	read_information(topo,edge_num,demand);
    printf("End");
	fclose(flog);
	while(1);
	return 0;
}

