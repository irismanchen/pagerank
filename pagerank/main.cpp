//
//  main.cpp
//  pagerank
//
//  Created by 王曼晨 on 15/10/14.
//  Copyright © 2015年 王曼晨. All rights reserved.
//

#include<cstdio>
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<string>
#include<algorithm>
#include<map>
#include<string.h>
#include<vector>
using namespace std;
#define MAX 1000000
struct edge	             //边信息
{
    int u;
    int v;
}edge[5200000];
int d[MAX]={0};		    //记录点的出度
double rankcur[MAX]={0};//记录点当前的pagerank值
double rankupd[MAX]={0};//记录点更新的pagerank值
int ncnt=0;          //记录点的个数
int ecnt=0;          //记录边的个数
map<string,int> url;
map<double,int> pr;
vector<string> vec;
void solve()
{
    ifstream fin("/Users/Iris/Desktop/pagerank/original-info.txt");
    ofstream fout("/Users/Iris/Desktop/pagerank/output.txt");
    memset(edge,0,sizeof(edge));
    string s,buf,tbuf;
    int cnt=0;
    double eps=0.1;
    int i,j;
    int ranksum=0;
    map<string,int>::iterator iter;
    map<double,int>::reverse_iterator iter1;
    while(getline(fin,buf))
    {
        if(buf[0]=='h')
        {
            j=(int)buf.find("\t");
            buf.erase(j,9);
            iter=url.find(buf);
            if(iter==url.end())
            {
                url.insert(pair<string,int>(buf,ncnt));
                vec.push_back(buf);
                ncnt++;
            }
            tbuf=buf;
        }
        else if(buf.length()==0)
        {
            continue;
        }
        else
        {
            buf=buf.erase(0,10);
            j=(int)buf.find("anchor");
            buf=buf.substr(0,j-1);
            iter=url.find(buf);
            if(iter==url.end())
            {
                url.insert(pair<string,int>(buf,ncnt));
                vec.push_back(buf);
                ncnt++;
            }
            edge[ecnt].v=url[tbuf];
            edge[ecnt++].u=url[buf];
            d[url[buf]]++;
        }
    }
    printf("%d %d\n",ncnt,ecnt);
    for(i=0;i<ncnt;++i)        //刚开始每个点被访问的概率一样都是1/ncnt
        rankcur[i]=(double)1/ncnt;
    int times;
    while(eps>0.0000001)       //set ε=10^(-7)
   // for(times=1;times<=30;times++)
    {
        printf("%d %.7lf\n",cnt,eps);
        eps=0;
        ranksum=0;
        cnt++;
        for(int i=0;i<ecnt;++i)
            rankupd[edge[i].v]+=rankcur[edge[i].u]/d[edge[i].u]; //计算出rankupd
        for(int i=0;i<ncnt;++i)
        {
            rankupd[i]=rankupd[i]*0.85+(0.15*1/ncnt); //α取0.8
            eps+=rankcur[i]>rankupd[i]?(rankcur[i]-rankupd[i]):(rankupd[i]-rankcur[i]);//计算rankcur和rankupd的差值，更新eps
            rankcur[i]=rankupd[i];
            rankupd[i]=0;
        }
    }
    for(i=0;i<ncnt;i++)
    {
        pr.insert(pair<double,int>(rankcur[i],i));
    }
    fout<<"url   |   pagerank"<<endl;
    for(iter1 = pr.rbegin() ; iter1 != pr.rend() ; iter1++)
    {
        fout<<vec[iter1->second]<<" "<<iter1->first<<endl;
    }
}
int main()
{
    solve();
    return 0;
}

