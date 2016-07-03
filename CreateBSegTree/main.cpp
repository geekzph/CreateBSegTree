//
//  main.cpp
//  CreateBSegTree
//
//  Created by zph on 16/5/29.
//  Copyright © 2016年 zph. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include "vector"
#include <time.h>
#include <stdlib.h>
using namespace std;

const int kBranchNum = 4; //branch number
typedef struct Node{    //Node structure
    int l;
    int r;
    int maxi,lmaxi,rmaxi,sum;
    struct Node *s[kBranchNum+1];
} Node;

typedef struct Kp{    //define pointer structer
    struct Node *s[kBranchNum+1];
} Kp;

int i = 0;
float * ReadDate(char filename[])
{
    i = 0;
    float r[20000];
    FILE *fp;
    double result = 0.0;
    char strline[8];                        //each line's max-character
    if((fp = fopen(filename,"r")) == NULL)
    {
        printf("error!");
        return NULL;
    }
    while (!feof(fp))
    {
        fgets(strline,12,fp);  //read a line
        //printf("%s", strline);
        result=atof(strline);
        i++;
        r[i] =result;
        
        
    }
    fclose(fp);              //close file
    printf("success\n");
    return r;
}

int max(int a, int b)
{
    return a>b?a:b;
}

//merge multi-branch in CreateTree
void MergeBranchInC(Node *p,int n)
{
    int psum=0,pmaxi=0,plmaxi=0,prmaxi=0;
    for (int i = 1; i < n; i++)
    {
        if (i == 1)
        {
            p->sum = p->s[1]->sum + p->s[2]->sum;
            p->maxi = max(p->s[1]->maxi,max(p->s[2]->maxi,p->s[1]->rmaxi+p->s[2]->lmaxi));
            p->lmaxi = max(p->s[1]->lmaxi,p->s[1]->sum + p->s[2]->lmaxi);
            p->rmaxi = max(p->s[2]->rmaxi,p->s[2]->sum + p->s[1]->rmaxi);
            psum=p->sum ,pmaxi=p->maxi,plmaxi=p->lmaxi,prmaxi=p->rmaxi;
        }
        else if(i > 1)
        {
            int m = i + 1;
            p->sum = psum + p->s[m]->sum;
            p->maxi = max(pmaxi,max(p->s[m]->maxi,prmaxi+p->s[m]->lmaxi));
            p->lmaxi = max(plmaxi,psum + p->s[m]->lmaxi);
            p->rmaxi = max(p->s[m]->rmaxi,p->s[m]->sum + prmaxi);
            psum=p->sum ,pmaxi=p->maxi,plmaxi=p->lmaxi,prmaxi=p->rmaxi;
        }
        
    }
    
}



//merge branch in query funcution
//query funtion's merge procedure is different from createtree's merge procedure
Node *MergeBranchInQ(Node *p, Kp *k, Node *q, int n)
{
    Node *res = (Node *)malloc(sizeof(Node));
    
    if(n ==1 )
    {
        res->sum = p->sum + q->sum;
        res->lmaxi = max(p->lmaxi,p->sum + q->lmaxi);
        res->rmaxi = max(q->rmaxi,q->sum + p->rmaxi);
        res->maxi = max(p->rmaxi+q->lmaxi,max(p->maxi,q->maxi));
        return res;

    }
    else
    {
        int psum=0,pmaxi=0,plmaxi=0,prmaxi=0;
        for (int i = 1; i < n; i++)
        {
            if (i == 1)
            {
                res->sum = p->sum + k->s[1]->sum;
                res->lmaxi = max(p->lmaxi,p->sum + k->s[1]->lmaxi);
                res->rmaxi = max(k->s[1]->rmaxi,k->s[1]->sum + p->rmaxi);
                res->maxi = max(p->rmaxi+k->s[1]->lmaxi,max(p->maxi,k->s[1]->maxi));
                psum=res->sum ,pmaxi=res->maxi,plmaxi=res->lmaxi,prmaxi=res->rmaxi;
            }
            else if(i > 1)
            {
                res->sum = psum + k->s[i]->sum;
                res->lmaxi = max(plmaxi,psum + k->s[i]->lmaxi);
                res->rmaxi = max(k->s[i]->rmaxi,k->s[i]->sum + prmaxi);
                res->maxi = max(prmaxi+k->s[i]->lmaxi,max(pmaxi,k->s[i]->maxi));
                psum=res->sum ,pmaxi=res->maxi,plmaxi=res->lmaxi,prmaxi=res->rmaxi;
            }
            
        }
        res->sum = psum + q->sum;
        res->lmaxi = max(plmaxi,psum + q->lmaxi);
        res->rmaxi = max(q->rmaxi,q->sum + prmaxi);
        res->maxi = max(prmaxi+q->lmaxi,max(pmaxi,q->maxi));
        return res;
    }
    
}

//create multi-branch tree
void CreateTree(int l ,int r , Node *tp, float x[])
{
    int i = 1;
    int ll = l;
    int rr = r;
    tp->l = l;
    tp->r = r;
    int seg = (r - l) / kBranchNum;   //segment length
    int flag = 0;
    
    if(ll == rr)
    {
        tp->l = ll;
        tp->r = rr;
        tp->maxi = tp->lmaxi = tp->rmaxi = tp->sum = x[ll];
        return;
    }
    
    for (i ; i < kBranchNum + 1; ++i)
    {
        seg = (r - l) / kBranchNum;
        if (ll > r || flag == 1)
        {
            break;
        }
        else if (ll + seg > r && ll < r + 1)
        {
            tp->s[i] = (Node *)malloc(sizeof(Node));  //apply for node
            CreateTree(ll, r,tp->s[i],x);             //create node
            flag = 1;                                 //quit for
        }
        else
        {
            tp->s[i] = (Node *)malloc(sizeof(Node));   //apply for node
            CreateTree(ll, ll + seg,tp->s[i],x);       //create node
            ll = ll + seg + 1;
            rr = ll + seg ;
        }
        
    }
    MergeBranchInC(tp, i - 1);
}

int IntervalNum(int x,Node *p)
{
    int m = 0;
    for(int i = 1;p->s[i]!=NULL && i <= kBranchNum; i++)
    {
        if (x >= p->s[i]->l && x <= p->s[i]->r) m = i;
    }
    return m;
}

Node *QuerySeg(int l,int r,int aa,int bb, Node *tp,int num)
{
    int flag1 = 0;
    int flag2 = 0;
    if(num != 0) tp = tp -> s[num];
    Node *ka, *kl, *kr, *res, *res1;
    Kp *k = (Kp *)malloc(sizeof(Kp));
    res = (Node *)malloc(sizeof(Node));
    if(aa <= l && bb >= r)
        return tp;
    int ll = 0;
    int rr = 0;
    ll = IntervalNum(aa, tp);
    rr = IntervalNum(bb, tp);
    if(ll == rr)
    {
        if(tp -> s[rr] -> r < bb)
            ka = QuerySeg(tp -> s[ll] ->r,tp -> s[rr]->r, aa, tp -> s[rr]->r, tp, ll);
        else
            ka = QuerySeg(tp -> s[ll] ->l,tp -> s[rr]->r, aa, bb, tp, ll);
        flag1 = 1;
    }
    int i = 1;
    if(ll < rr)
    {
        kl = QuerySeg(tp -> s[ll] ->l,tp -> s[ll] ->r, aa, tp -> s[ll] ->r, tp, ll);//lefmost point
        while (ll < rr - 1)
        {
            k -> s[i] = QuerySeg(tp -> s[ll+1] ->l,tp -> s[ll+1] ->r, tp -> s[ll+1] ->l, tp -> s[ll+1] ->r, tp, ll+1);
            ll++;
            i++;
        }
        kr = QuerySeg(tp -> s[rr] ->l,tp -> s[rr] ->r, tp -> s[rr] ->l, bb, tp, rr);//rightmost point
        flag2 = 1;
        
    }
    if (flag1 == 1)
    {
        res = ka;
    }
    else if(flag2 == 1)
    {
        res1 = MergeBranchInQ(kl, k, kr, i);
        res -> sum = res1 -> sum;
        res -> lmaxi = res1 -> lmaxi;
        res -> rmaxi = res1 -> rmaxi;
        res -> maxi = res1 -> maxi;
    }
    return res;
}

//void LevelTra3(Node* root)
//{
//    if (!root)
//    {
//        return;
//    }
//    
//    queue<int> m_queue;
//    m_queue.push(root);
//    while (!m_queue.empty())
//    {
//        Node* cur=m_queue.front();
//        cout<<cur->m_data<<"  ";
//        m_queue.pop();
//        if (cur->m_lChild)
//        {
//            m_queue.push(cur->m_lChild);
//        }
//        if (cur->m_rChild)
//        {
//            m_queue.push(cur->m_rChild);
//        }
//    }
//    cout<<endl;
//}

//void LevelTra4(Node* root)
//{
//    if (!root)
//    {
//        return;
//    }
//    vector<Node*> vec;
//    vec.push_back(root);
//    int cur=0;
//    int last=1;
//    while (cur<vec.size())
//    {
//        last=vec.size();
//        while (cur<last)
//        {
//            cout<<vec[cur]->sum<<"  ";
//            if (vec[cur]->s1)
//            {
//                vec.push_back(vec[cur]->s1);
//            }
//            if (vec[cur]->s2)
//            {
//                vec.push_back(vec[cur]->s2);
//            }
//            if (vec[cur]->s3)
//            {
//                vec.push_back(vec[cur]->s3);
//            }
//            if (vec[cur]->s4)
//            {
//                vec.push_back(vec[cur]->s4);
//            }
//
//            cur++;
//        }
//        cout<<endl;
//    }
//}
Node *rootnode = (Node *)malloc(sizeof(Node)); //apply for root node

int main(int argc, const char * argv[]) {
    // insert code here...
    //int n = IntervalNum(0, 25, 4, 25);
    float *p;
    p = ReadDate("/Users/zph/XcodeProject/编程珠玑/SegTreeMaxSub/SegTreeMaxSub/data.txt");
    printf("total data is %d\n",i);
    CreateTree(1, i, rootnode, p);
    //LevelTra4(RootNode);
    Node *res = QuerySeg(1, i, 123, 1000, rootnode, 0);
    printf("maxsub sum is :%d\n",res->maxi);
    return 0;
}
