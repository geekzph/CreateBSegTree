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

typedef struct Node{    /* 定义单链表结点类型 */
    int L;
    int R;
    int maxi,lmaxi,rmaxi,sum;
    struct Node *s1;
    struct Node *s2;
    struct Node *s3;
    struct Node *s4;
    struct Node *s[11];
} Node;

int i = 0;
float * ReadDate(char filename[])
{
    i = 0;
    float r[20000];
    FILE *fp;
    double result = 0.0;
    char StrLine[8];             //每行最大读取的字符数
    if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
    {
        printf("error!");
        return NULL;
    }
    while (!feof(fp))
    {
        fgets(StrLine,12,fp);  //读取一行
        //printf("%s", StrLine);
        result=atof(StrLine);
        i++;
        r[i] =result;
        
        
    }
    fclose(fp);
    //关闭文件
    printf("success\n");
    return r;
}

int max(int a, int b)
{
    return a>b?a:b;
}

//合并两个分支
void pushup2(Node *p)
{
    p->sum = p->s1->sum + p->s2->sum;
    p->maxi = max(p->s1->maxi,max(p->s2->maxi,p->s1->rmaxi+p->s2->lmaxi));
    p->lmaxi = max(p->s1->lmaxi,p->s1->sum + p->s2->lmaxi);
    p->rmaxi = max(p->s2->rmaxi,p->s2->sum + p->s1->rmaxi);
}
//合并三个分支
void pushup3(Node *p)
{
    p->sum = p->s1->sum + p->s2->sum;
    p->maxi = max(p->s1->maxi,max(p->s2->maxi,p->s1->rmaxi+p->s2->lmaxi));
    p->lmaxi = max(p->s1->lmaxi,p->s1->sum + p->s2->lmaxi);
    p->rmaxi = max(p->s2->rmaxi,p->s2->sum + p->s1->rmaxi);
    
    int psum=p->sum ,pmaxi=p->maxi,plmaxi=p->lmaxi,prmaxi=p->rmaxi;
    p->sum = psum + p->s3->sum;
    p->maxi = max(pmaxi,max(p->s3->maxi,prmaxi+p->s3->lmaxi));
    p->lmaxi = max(plmaxi,psum + p->s3->lmaxi);
    p->rmaxi = max(p->s3->rmaxi,p->s3->sum + prmaxi);
}
//合并四个分支
void pushup4(Node *p)
{
    p->sum = p->s1->sum + p->s2->sum;
    p->maxi = max(p->s1->maxi,max(p->s2->maxi,p->s1->rmaxi+p->s2->lmaxi));
    p->lmaxi = max(p->s1->lmaxi,p->s1->sum + p->s2->lmaxi);
    p->rmaxi = max(p->s2->rmaxi,p->s2->sum + p->s1->rmaxi);
    
    int psum=p->sum ,pmaxi=p->maxi,plmaxi=p->lmaxi,prmaxi=p->rmaxi;
    p->sum = psum + p->s3->sum;
    p->maxi = max(pmaxi,max(p->s3->maxi,prmaxi+p->s3->lmaxi));
    p->lmaxi = max(plmaxi,psum + p->s3->lmaxi);
    p->rmaxi = max(p->s3->rmaxi,p->s3->sum + prmaxi);
    
    psum=p->sum ,pmaxi=p->maxi,plmaxi=p->lmaxi,prmaxi=p->rmaxi;
    p->sum = psum + p->s4->sum;
    p->maxi = max(pmaxi,max(p->s4->maxi,prmaxi+p->s4->lmaxi));
    p->lmaxi = max(plmaxi,psum + p->s4->lmaxi);
    p->rmaxi = max(p->s4->rmaxi,p->s4->sum + prmaxi);


}

//创建多分支线段树
int branchnum = 10;
void CreateTree(int l ,int r , Node *tp, float x[])
{
    int i = 1;
    int ll = l;
    int rr = r;
    tp->L = l;
    tp->R = r;
    int seg = (r - l) / branchnum;//段长
    int flag = 0;
    
    if(ll == rr)
    {
        tp->L = ll;
        tp->R = rr;
        tp->maxi = tp->lmaxi = tp->rmaxi = tp->sum = x[ll];
        return;
    }
    
    for (i ; i < branchnum + 1; ++i)
    {
        seg = (r - l) / branchnum;
        if (ll > r || flag == 1)
        {
            break;
        }
        else if (ll + seg > r && ll < r + 1)
        {
            tp->s[i] = (Node *)malloc(sizeof(Node));  //申请节点
            CreateTree(ll, r,tp->s[i],x);              //创建节点
            flag = 1;                                 //quit for
        }
        else
        {
            tp->s[i] = (Node *)malloc(sizeof(Node));  //申请节点
            CreateTree(ll, ll + seg,tp->s[i],x);        //创建节点
            ll = ll + seg + 1;
            rr = ll + seg ;
        }
        
    }
//    if(flag == 3 || flag > 4)
//    {
//        tp->s1 = (Node *)malloc(sizeof(Node)); //申请s1
//        CreateTree(l, l + seg,tp->s1,x);
//        tp->s2 = (Node *)malloc(sizeof(Node)); //申请s2
//        CreateTree(l + seg + 1, l + 2 * seg + 1,tp->s2,x);
//        tp->s3 = (Node *)malloc(sizeof(Node)); //申请s3
//        CreateTree(l + 2 * seg + 2, l + 3 * seg + 2,tp->s3,x);
//        tp->s4 = (Node *)malloc(sizeof(Node)); //申请s4
//        CreateTree(l + 3 * seg + 3, r, tp->s4,x);
//        pushup4(tp);
//    }
//    if (flag == 4)
//    {
//        tp->s1 = (Node *)malloc(sizeof(Node)); //申请s1
//        CreateTree(l, l+1, tp->s1,x);
//        tp->s2 = (Node *)malloc(sizeof(Node)); //申请s2
//        CreateTree(l + 2, l + 3, tp->s2,x);
//        tp->s3 = (Node *)malloc(sizeof(Node)); //申请s3
//        CreateTree(l + 4, l + 4, tp->s3,x);
//        tp->s4 = NULL;
//        pushup3(tp);
//    }
//    if (flag == 2)
//    {
//        tp->s1 = (Node *)malloc(sizeof(Node)); //申请s1
//        CreateTree(l, l, tp->s1,x);
//        tp->s2 = (Node *)malloc(sizeof(Node)); //申请s2
//        CreateTree(l + 1, l + 1, tp->s2,x);
//        tp->s3 = (Node *)malloc(sizeof(Node)); //申请s3
//        CreateTree(l + 2, l + 2, tp->s3,x);
//        tp->s4 = NULL;
//        pushup3(tp);
//    }
//    if (flag == 1)
//    {
//        tp->s1 = (Node *)malloc(sizeof(Node)); //申请s1
//        CreateTree(l, l, tp->s1,x);
//        tp->s2 = (Node *)malloc(sizeof(Node)); //申请s2
//        CreateTree(l + 1, l + 1, tp->s2,x);
//        tp->s3 = NULL;
//        tp->s4 = NULL;
//        pushup2(tp);
//    }
//    
    
}

int IntervalNum(int l,int r,int n,int num)
{
    int seg = (r - l)/4;
    if(n==4)
    {
        if(num >l - 1 && num<l + seg + 1)
            return 1;
        else if(num > l + seg && num<l + 2 * seg + 2)
            return 2;
        else if(num > l + 2 * seg + 1 && num<l + 3 * seg + 3)
            return 3;
        else if(num > l + 3 * seg + 2)
            return 4;
        else
            return -1;
    }
    else if(n==3)
    {
        if(num >l - 1 && num<l + seg + 1)
            return 1;
        else if(num > l + seg && num<l + 2 * seg + 2)
            return 2;
        else if(num > l + 2 * seg + 1 && num<l + 3 * seg + 3)
            return 3;
        else
            return -1;

        
    }
    else if(n == 2)
    {
        if(num >l - 1 && num<l + seg + 1)
            return 1;
        else if(num > l + seg && num<l + 2 * seg + 2)
            return 2;
        else
            return -1;
    }
    else
        return -1;
}

Node *ChangePointer(Node *tp,int snum)
{
    Node *p = NULL;
    if(snum == 1)  p = tp -> s1;
    if(snum == 2)  p = tp -> s2;
    if(snum == 3)  p = tp -> s3;
    if(snum == 4)  p = tp -> s4;
    return p;
}

Node *query(int l,int r,int aa,int bb, Node *tp,int num)
{
    if(num ==1) tp = tp -> s1;
    else if(num==2) tp = tp -> s2;
    else if(num==3) tp = tp -> s3;
    else if(num==4) tp = tp -> s4;
    Node *ka,*kb,*res;
    res = (Node *)malloc(sizeof(Node));
    if(aa <= l && bb >= r)
       return tp;
    //int seg = (r - l)/4;
    int flag = r - l;//区间个数
    int flag1 = 0;   //区间合并标志
    int flag2 = 0;
    int flag4 = 0;
    int snum1 = -1;
    int snum2 = -1;
    Node *p = NULL;
    if(flag == 3 || flag > 4)
    {// 四个区间
        snum1 = IntervalNum(l, r, 4, aa);  //区间指针
        snum2 = IntervalNum(l, r, 4, bb);
        if(snum1 != snum2 && snum1 > 0 && snum2 > 0)
        {
            int m = snum2 - snum1;//相隔的区间数
            p = ChangePointer(tp, snum1);
            ka = query(p -> L, p -> R, aa, bb, tp, snum1);
            p = ChangePointer(tp, snum2);
            kb = query(p -> L, p -> R, p -> L, bb, tp, snum2);
            if(m == 1 ) flag1 = flag2 = 1;
            if(m == 2) {flag4 = 2;}
            if(m == 3) {flag4 = 3;}
        }
        else if(snum1 > 0)
        {
            p = ChangePointer(tp, snum1);
            ka = query(p -> L, p -> R, aa, bb, tp, snum1);
            flag1 = 0;
        }
        else if(snum2 > 0)
        {
            p = ChangePointer(tp, snum2);
            ka = query(p -> L, p -> R, aa, bb, tp, snum2);
            flag1 = 0;
        }

        
        
    }
    if(flag == 2 || flag ==4)
    {//三个区间
        snum1 = IntervalNum(l, r, 3, aa);
        snum2 = IntervalNum(l, r, 3, bb);
        //Node *p = NULL;
        if(snum1 != snum2 && snum1 > 0 && snum2 > 0)
        {
            int m = snum2 - snum1;//相隔的区间数
            p = ChangePointer(tp, snum1);
            ka = query(p -> L, p -> R, aa, bb, tp, snum1);
            p = ChangePointer(tp, snum2);
            kb = query(p -> L, p -> R, p -> L, bb, tp, snum2);
            if(m == 1 ) flag1 = flag2 = 1;
            if(m == 2) {flag4 = 2;}
            //if(m == 3) {flag4 = 3;}
        }
        else if(snum1 > 0)
        {
            p = ChangePointer(tp, snum1);
            ka = query(p -> L, p -> R, aa, bb, tp, snum1);
            flag1 = 0;
        }
        else if(snum2 > 0)
        {
            p = ChangePointer(tp, snum2);
            ka = query(p -> L, p -> R, aa, bb, tp, snum2);
            flag1 = 0;
        }
        
    }
    if(flag == 1)
    {//两个区间
        snum1 = IntervalNum(l, r, 2, aa);
        snum2 = IntervalNum(l, r, 2, bb);
        //Node *p = NULL;
        if(snum1 != snum2 && snum1 > 0 && snum2 > 0)
        {
            int m = snum2 - snum1;//相隔的区间数
            p = ChangePointer(tp, snum1);
            ka = query(p -> L, p -> R, aa, bb, tp, snum1);
            p = ChangePointer(tp, snum2);
            kb = query(p -> L, p -> R, p -> L, bb, tp, snum2);
            if(m == 1 ) flag1 = flag2 = 1;
            //if(m == 2) {flag4 = 2;}
            //if(m == 3) {flag4 = 3;}
        }
        else if(snum1 > 0)
        {
            p = ChangePointer(tp, snum1);
            ka = query(p -> L, p -> R, aa, bb, tp, snum1);
            flag1 = 0;
        }
        else if(snum2 > 0)
        {
            p = ChangePointer(tp, snum2);
            ka = query(p -> L, p -> R, aa, bb, tp, snum2);
            flag1 = 0;
        }

    }
    ////////////////////////////////////////////////////////////
    //区间合并计算
    if(flag1 && flag2)
    {
        res->sum = ka->sum + kb->sum;
        res->lmaxi = max(ka->lmaxi,ka->sum+kb->lmaxi);
        res->rmaxi = max(kb->rmaxi,kb->sum+ka->rmaxi);
        res->maxi = max(ka->rmaxi+kb->lmaxi,max(ka->maxi,kb->maxi));
    }
    else if (flag4 == 2)
    {
        p = ChangePointer(tp, snum1 + 1);
        res->sum = ka->sum + p->sum;
        res->lmaxi = max(ka->lmaxi,ka->sum+p->lmaxi);
        res->rmaxi = max(p->rmaxi,p->sum+ka->rmaxi);
        res->maxi = max(ka->rmaxi+p->lmaxi,max(ka->maxi,p->maxi));
        
        int psum=res->sum ,pmaxi=res->maxi,plmaxi=res->lmaxi,prmaxi=res->rmaxi;
        res->sum = psum + kb->sum;
        res->lmaxi = max(plmaxi,psum+kb->lmaxi);
        res->rmaxi = max(kb->rmaxi,kb->sum+prmaxi);
        res->maxi = max(prmaxi+kb->lmaxi,max(pmaxi,kb->maxi));

    }
    else if (flag4 == 3)
    {
        p = ChangePointer(tp, snum1 + 1);
        res->sum = ka->sum + p->sum;
        res->lmaxi = max(ka->lmaxi,ka->sum+p->lmaxi);
        res->rmaxi = max(p->rmaxi,p->sum+ka->rmaxi);
        res->maxi = max(ka->rmaxi+p->lmaxi,max(ka->maxi,p->maxi));
        
        int psum=res->sum ,pmaxi=res->maxi,plmaxi=res->lmaxi,prmaxi=res->rmaxi;
        p = ChangePointer(tp, snum1 + 2);
        res->sum = psum + p->sum;
        res->lmaxi = max(plmaxi,psum+p->lmaxi);
        res->rmaxi = max(p->rmaxi,p->sum+prmaxi);
        res->maxi = max(prmaxi+p->lmaxi,max(pmaxi,p->maxi));
        
        psum=res->sum ,pmaxi=res->maxi,plmaxi=res->lmaxi,prmaxi=res->rmaxi;
        res->sum = psum + kb->sum;
        res->lmaxi = max(plmaxi,psum+kb->lmaxi);
        res->rmaxi = max(kb->rmaxi,kb->sum+prmaxi);
        res->maxi = max(prmaxi+kb->lmaxi,max(pmaxi,kb->maxi));

    }
    else
    {
        if(flag1==0)  //left
            res = ka;
        else
            res = kb;
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

void LevelTra4(Node* root)
{
    if (!root)
    {
        return;
    }
    vector<Node*> vec;
    vec.push_back(root);
    int cur=0;
    int last=1;
    while (cur<vec.size())
    {
        last=vec.size();
        while (cur<last)
        {
            cout<<vec[cur]->sum<<"  ";
            if (vec[cur]->s1)
            {
                vec.push_back(vec[cur]->s1);
            }
            if (vec[cur]->s2)
            {
                vec.push_back(vec[cur]->s2);
            }
            if (vec[cur]->s3)
            {
                vec.push_back(vec[cur]->s3);
            }
            if (vec[cur]->s4)
            {
                vec.push_back(vec[cur]->s4);
            }

            cur++;
        }
        cout<<endl;
    }
}
Node *RootNode = (Node *)malloc(sizeof(Node)); //申请新节点;

int main(int argc, const char * argv[]) {
    // insert code here...
    //int n = IntervalNum(0, 25, 4, 25);
    float *p;
    p = ReadDate("/Users/zph/XcodeProject/编程珠玑/SegTreeMaxSub/SegTreeMaxSub/100.txt");
    printf("一共%d个数据\n",i);
    CreateTree(1 , 100, RootNode, p);
    LevelTra4(RootNode);
    //Node *res = query(1, i, 1,19000  , RootNode,0);
    //printf("maxsub sum is :%d\n",res->maxi);
    return 0;
}
