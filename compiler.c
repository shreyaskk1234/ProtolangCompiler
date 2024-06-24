#include <stdio.h>
#include <string.h>
#include <stdlib.h>
FILE* fp;
FILE* outfp;
FILE* temp;
short codesection=0;
char l;
int ln=0;
char token[256];
char num[256];
void* procname;
char op[3];
long long n=0;
short pno=0;
struct identifier
{
    char id[256];
    char idt;
    short pbytes;
    short nbytes;
    struct identifier *next;
    struct localident *locltblst;
    struct localident *locltblcurr;
};
struct identifier *curr=NULL;
struct identifier *start;
struct localident
{
    char id[256];
    char idt;
    short no;
    short nb;
    struct localident *next;
};
char add(char t1);
char and(char t1);
void assign(void* t,void* lt);
void assignorproc(void *lt);
char bitexpr();
char bitterm();
void block(void* lt,char* Label);
char booland();
char booleor();
char boolexpr();
void openfile(char* filename);
void closefile();
char readchar();
short IsDigit(char c);
short isAdOp(char c);
short IsAlph(char c);
short IsAlnum_(char c);
short IsSpace(char c);
void convert(char source,char dest,char reg);
void skipspace();
void getword();
void getnum();
void getop();
void error(char* s);
void expected(char* s);
short Match(char* t);
short MatchChar(char* t);
void emit(char* s);
void emitln(char* s);
char factor();
char mul(char t1);
char divide(char t1);
char term();
char add(char t1);
char sub(char t1);
char expr();
void decl();
void assign(void *t,void *lt);
void condition();
void ifst(void* lt,char* Label);
void block(void* lt,char* Label);
char negfactor();
void elsest(void* lt,char* Label);
void whilest(void* lt);
char boolfactor();
char notfactor();
char boolterm();
char boolexpr();
char* NewLabel();
void freeLabel(char* L);
void var_decl();
void MainProg();
void proc();
char bitterm();
char bitexpr();
void openfile(char* filename)
{
    fp=fopen(filename,"r");
}
void writefile(char* filename)
{
    outfp=fopen(filename,"w+");
}
void closefile()
{
    fclose(fp);
    fclose(outfp);
}
char readchar()
{
    l=fgetc(fp);
    return l;
}

short IsDigit(char c)
{
    if(c>='0'&& c<='9')
        return 1;
    else
        return 0;
}
short isAdOp(char c)
{
    if(c=='+'||c=='-')
        return 1;
    else
        return 0;
}

short isMuOp(char c)
{
    if(c=='*'||c=='/')
        return 1;
    else
        return 0;
}
short isReOp(char* s)
{
    if((strcmp(s,">")==0)||(strcmp(s,"<")==0)||(strcmp(s,"==")==0)||(strcmp(s,"!=")==0)||(strcmp(s,"<=")==0)||(strcmp(s,">=")==0))
        return 1;
    else
        return 0;
}

short IsAlph(char c)
{
    if(c>='A'&& c<='Z'||c>='a'&& c<='z')
        return 1;
    else
        return 0;
}

short IsAlnum_(char c)
{
    if(IsAlph(c)||IsDigit(c)||c=='_')
        return 1;
    else
        return 0;
}

short IsSpace(char c)
{
    if(c==' '||c=='\t'||c=='\r'||c=='\n')
        return 1;
    else
        return 0;
}

short IsBool(char c)
{
    if((c=='T')||(c=='F'))
        return 1;
    else
        return 0;
}

short getbool()
{
    if(IsBool(l))
    {
    if(l=='T')
    {
        readchar();
        return 1;
    }
    else if(l=='F')
     {
         readchar();
         return 0;
     }
    }
    else
        return -1;

}
void skipspace()
{
    if(IsSpace(l))
    while(IsSpace(l))
        readchar();
}

void getword()
{
    short i=0;
    skipspace();
    strcpy(token,"");
    if(IsAlnum_(l))
    {
    while(IsAlnum_(l))
    {
        token[i++]=l;
        l=readchar();
    }
    token[i]='\0';
    }
    else
    expected("identifier");
    skipspace();
}

void getnum()
{
    short d=0,k=0;
    short i=0;
    skipspace();
    strcpy(num,"");
    n=0;
    if(IsDigit(l))
    {
        while(IsDigit(l))
        {
            num[i++]=l;
            l=readchar();
        }
        num[i]='\0';
        while(k<i)
        {
            d=num[k++]-'0';
            n=n*10+d;
        }
    }
     else
    expected("number");
    skipspace();
}

void getop()
{
    skipspace();
    op[0]=l;
    if(op[0]=='>'||op[0]=='<'||op[0]=='='||op[0]=='!')
    {
        readchar();
        if(l=='=')
        {
            op[1]=l;
            op[2]='\0';
        }
        else
        {
        op[1]='\0';
        fseek(fp,-1,1);
        /*readchar();*/
        }
    }
    else if(op[0]=='&'||op[0]=='|')
    {
        readchar();
        if(op[0]==l)
        {
            op[1]=l;
            op[2]='\0';
        }
        else
        {
        op[1]='\0';
        fseek(fp,-1,1);
        /*readchar();*/
        }
    }
    else
    op[1]='\0';
    skipspace();
}
void error(char* s)
{
    char tmp[40];
    sprintf(tmp,"Error: %s\n",s);
    printf("%s",tmp);
}

void expected(char* s)
{
    char tmp[40];
    sprintf(tmp,"Expected %s",s);
    error(tmp);
}
short Match(char* t)
{
    if(strcmp(token,t)==0)
    return 1;
    else
    {
    expected(t);
    return 0;
    }
}
short MatchChar(char* t)
{
    if(strcmp(op,t)==0)
    {
        readchar();
        return 1;
    }
    else
    {
    expected(t);
    return 0;
    }
}
short MatchSym(char t)
{
    char tmp[2];
    skipspace();
    if(l==t)
    {
        readchar();
        return 1;
    }
    else
    {
        if(t!=',')
        {
        sprintf(tmp,"%c",t);
        expected(tmp);
        }
        return 0;
    }
    skipspace();
}
void emit(char* s)
{
    fprintf(outfp,"\t%s",s);
}

void emitln(char* s)
{
    emit(s);
    fprintf(outfp,"\n");
}

void* createid(char t)
{
    struct identifier *temp;
    if(curr==NULL)
    {
    curr=malloc(sizeof(struct identifier));
    start=curr;
    }
    else
    {
    temp=malloc(sizeof(struct identifier));
    curr->next=temp;
    curr=temp;
    }
    curr->locltblst=NULL;
    curr->locltblcurr=NULL;
    strcpy(curr->id,token);
    curr->idt=t;
    if(t=='p')
    curr->nbytes=0;
    curr->next=NULL;
    return (void *)curr;
}

void* findtype()
{
    struct identifier *ct;
    ct=start;
    while(ct!=NULL)
    {
        if(strcmp(ct->id,token)==0)
            return((void*)ct);
            ct=ct->next;
    }
}

void* creatlid(char t,void *pr,void *cr)
{
    short bytes;
    char pt;
    struct localident *temp;
    struct identifier *p;
    struct localident *c;
    struct localident *temp1;
    p=(struct identifier *)pr;
    c=(struct localident *)cr;
    if(p->locltblst==NULL)
    {
    p->locltblcurr=malloc(sizeof(struct localident));
    p->locltblst=p->locltblcurr;
    strcpy( p->locltblcurr->id,token);
    p->locltblcurr->idt=t;
    p->locltblcurr->next=NULL;
    pno=1;
    p->locltblcurr->no=pno;
    c=p->locltblst;
    }
    else
    {
    temp=malloc(sizeof(struct localident));
    bytes=c->nb;
    pt=c->idt;
    c->next=temp;
    c=temp;
    strcpy(c->id,token);
    c->no=(++pno);
    c->idt=t;
    c->next=NULL;
    }
    return (void *)c;
}
void* findpvartype(void* proc,short* na,short np )
{
    struct identifier *p;
    struct localident *c;
    p=(struct identifier *)proc;
    c=p->locltblst;
    *na=0;
    while(c!=NULL)
    {
        if((c->no)==np)
        return((void*)c);
        c=c->next;
        (*na)++;
    }
    if(c==NULL)
    {
        error("Identifier not declared");
    }
}
void* findpvtype(void* proc,short *na)
{
    struct identifier *p;
    struct localident *c;
    p=(struct identifier *)proc;
    c=p->locltblst;
    *na=0;
    while(c!=NULL)
    {
        if(strcmp((c->id),token)==0)
        return((void*)c);
        c=c->next;
        (*na)++;
    }
    if(c==NULL)
    {
        error("Identifier not declared");
    }
}
void delid()
{
    struct identifier *ct;
    struct identifier *nt;
    struct localident *lt;
    struct localident *nlt;
    ct=start;
    while(ct!=NULL)
    {
        if(ct->idt=='p')
        {
        lt=ct->locltblst;
        while(lt!=NULL)
        {
            nlt=lt->next;
            free(lt);
            lt=nlt;
        }
        }
        nt=ct->next;
        free(ct);
        ct=nt;
    }
}
void convert(char source,char dest,char reg)
{
    char tmp[256];
    if(source!=dest)
    {
        if(source=='b'&&dest=='q')
        sprintf(tmp,"movsx r%cx,%cl",reg,reg);
        else if(source=='b'&&dest=='d')
        sprintf(tmp,"movsx e%cx,%cl",reg,reg);
        else if(source=='b'&&dest=='w')
        sprintf(tmp,"movsx %cx,%cl",reg,reg);
        else if(source=='w'&&dest=='q')
        sprintf(tmp,"movsx r%cx,%cx",reg,reg);
        else if(source=='w'&&dest=='d')
        sprintf(tmp,"movsx e%cx,%cx",reg,reg);
        else if(source=='d'&&dest=='q')
        sprintf(tmp,"movsxd r%cx,e%cx",reg,reg);
        else
        error("Incompatible types.");
        emitln(tmp);
    }
}
void regname(char* rname,char r,char c)
{
    char tmp[5],rn[5];
    if(c=='b')
        sprintf(tmp,"%cl",r);
    else if(c=='w')
        sprintf(tmp,"%cx",r);
    else if(c=='d')
        sprintf(tmp,"e%cx",r);
    else if(c=='q')
        sprintf(tmp,"r%cx",r);
    strcpy(rname,tmp);
}
char factor()
{
    char tmp[40],reg1[5],reg2[5],t;
    void* idft,*idct;
    struct identifier *ft;
    struct localident *ct;
    short na=0;
    skipspace();
    if(IsAlph(l)||l=='_')
    {
    getword();
    idft=findtype();
    if(idft!=NULL)
    {
    ft=(struct identifier *)idft;
    t=ft->idt;
    regname(reg1,'a',t);
    sprintf(tmp,"mov %s,[%s]",reg1,token);
    emitln(tmp);
    }
    else
    {
    idct=findpvtype(procname,&na);
    if(idct!=NULL)
    {
    ct=(struct localident*)idct;
    t=ct->idt;
    if(ct->nb>8)
    {
        regname(reg2,'b','q');
        sprintf(tmp,"mov %s,[rbp+%d]",reg2,(ct->nb));
        emitln(tmp);
        regname(reg1,'a',t);
        sprintf(tmp,"mov %s,[%s]",reg1,reg2);
        emitln(tmp);
    }
    else
    {
    regname(reg1,'a',t);
    sprintf(tmp,"mov %s,[rbp%d]",reg1,(ct->nb));
    emitln(tmp);
    }
    }
    else
    error("Identifier not declared");
    }
    }
    else if(IsDigit(l))
    {
    getnum();
    if(n>=0&&n<128)
    t='b';
    else if(n>=128&&n<32768)
    t='w';
    else if(n>=32768&&n<2147483648)
    t='d';
    else if(n>=2147483648LL&&n<=9223372036854775807LL)
    t='q';
    regname(reg1,'a',t);
    sprintf(tmp,"mov %s,%lld",reg1,n);
    emitln(tmp);
    }
    else if(l=='(')
    {
    getop();
    MatchChar("(");
    if(l=='-')
    t=negfactor();
    else
    bitexpr();
    getop();
    MatchChar(")");
    }
    skipspace();
    return t;
}

char negfactor()
{
    char tmp[256],reg[5],t;
    getop();
    MatchChar("-");
    if(IsDigit(l))
    {
    getnum();
    if(n>=0&&n<=128)
    t='b';
    else if(n>128&&n<=32768)
    t='w';
    else if(n>32768&&n<=2147483648)
    t='d';
    else if(n>2147483648LL&&n<=9223372036854775807LL)
    t='q';
    regname(reg,'a',t);
    sprintf(tmp,"mov %s,-%lld",reg,n);
    emitln(tmp);
    }
    else
    {
    t=factor();
    regname(reg,'a',t);
    sprintf(tmp,"neg %s",reg);
    emitln(tmp);
    }
    return t;
}

short compsize(char t1,char t2)
{
    if(t1=='q'&&(t2=='b'||t2=='w'||t2=='d'))
        return 1;
    if(t1=='d'&&(t2=='b'||t2=='w'))
        return 1;
    if(t1=='w'&&t2=='b')
        return 1;
    if(t2=='q'&&(t1=='b'||t1=='w'||t1=='d'))
        return 2;
    if(t2=='d'&&(t1=='b'||t1=='w'))
        return 2;
    if(t2=='w'&&t1=='b')
        return 2;
    if(t1==t2)
        return 0;
}
char samesize(char t1,char t2,char reg1,char reg2)
{
    char r;
    short s;
    s=compsize(t1,t2);
    if(s==1)
    {
        convert(t2,t1,reg2);
        r=t1;
    }
    else if(s==2)
    {
        convert(t1,t2,reg1);
        r=t2;
    }
    else
        r=t1;
        return r;
}
char mul(char t1)
{
    char t2,c,c1,reg1[5],reg2[5],tmp[20];
    short s;
    MatchChar("*");
    t2=factor();
    if(t1=='d'||t1=='q')
    regname(reg1,'c','q');
    else if(t1=='b'||t1=='w')
    regname(reg1,'c','w');
    sprintf(tmp,"pop %s",reg1);
    emitln(tmp);
    if(compsize(t1,t2)==1)
    {
    if(t1=='w')
        c='d';
    else if(t1=='d')
        c='q';
    if(t1!='q')
    convert(t1,c,'c');
    convert(t2,c,'a');
    }
    else if(compsize(t1,t2)==2)
    {
    if(t2=='w')
        c='d';
    else if(t2=='d')
        c='q';
    convert(t1,c,'c');
    if(t2!='q')
    convert(t2,c,'a');
    }
    else if(compsize(t1,t2)==0)
    {
    if(t1=='b')
        c='w';
    else if(t1=='w')
        c='d';
    else if(t1=='d')
        c='d';
    else if(t1=='q')
        c='q';
    if(t1!='q')
    {
    convert(t1,c,'c');
    convert(t2,c,'a');
    }
    }
    regname(reg1,'c',c);
    regname(reg2,'a',c);
    sprintf(tmp,"imul %s,%s",reg2,reg1);
    emitln(tmp);
    return c;
}
char divide(char t1)
{
    char t2,tmp[256],reg1[5],reg2[5],reg3[5],reg4[5];
    MatchChar("/");
    t2=factor();
    regname(reg2,'a',t2);
    regname(reg4,'b',t2);
    sprintf(tmp,"mov %s,%s",reg4,reg2);
    emitln(tmp);
    if(t1=='d'||t1=='q')
    regname(reg1,'c','q');
    else if(t1=='b'||t1=='w')
    regname(reg1,'c','w');
    sprintf(tmp,"pop %s",reg1);
    emitln(tmp);
    if((t1=='b')&&(t2=='b'))
    {
        convert(t1,'w','c');
        t1='w';
    }
    if((t1=='b')&&(t2=='w'))
    {
        convert(t1,'d','c');
        t1='d';
    }
    if((t1=='b')&&(t2=='d')||(t2=='q'))
    {
        convert(t1,'q','c');
        t1='q';
    }
    if((t1=='w')&&(t2=='w'))
    {
        convert(t1,'d','c');
        t1='d';
    }
    if(((t1=='w')||t1=='d')&&(t2=='d')||(t2=='q'))
    {
        convert(t1,'q','c');
        t1='q';
    }
    if((t1=='d')&&(t2=='b'))
    {
        convert(t2,'w','b');
        t2='w';
    }
    if((t1=='q')&&(t2=='b')||(t2=='w'))
    {
        convert(t2,'d','b');
        t2='d';
    }
    regname(reg1,'c',t1);
    regname(reg2,'a',t1);
    sprintf(tmp,"mov %s,%s",reg2,reg1);
    emitln(tmp);
    regname(reg3,'d',t1);
    if(t2!='q')
    sprintf(tmp,"mov %s,%s",reg3,reg1);
    else
    sprintf(tmp,"mov %s,0",reg3);
    emitln(tmp);
    if(t1=='d')
    {
    sprintf(tmp,"sar %s,16",reg3);
    emitln(tmp);
    sprintf(tmp,"and %s,65535",reg2);
    emitln(tmp);
    }
    if(t1=='q')
    {
    if(t2!='q')
    {
    sprintf(tmp,"sar %s,32",reg3);
    emitln(tmp);
    emitln("mov r8,4294967295");
    sprintf(tmp,"and %s,r8",reg2);
    emitln(tmp);
    }
    }
    sprintf(tmp,"idiv %s",reg4);
    emitln(tmp);
    return t2;
}
char term()
{
    char t,reg1[5],tmp[256];
    t=factor();
    getop();
    while(isMuOp(op[0]))
    {
    if(t=='d'||t=='q')
    regname(reg1,'a','q');
    else if(t=='b'||t=='w')
    regname(reg1,'a','w');
    sprintf(tmp,"push %s",reg1);
    emitln(tmp);
    switch(op[0])
    {
        case '*':t=mul(t);
        break;
        case '/':t=divide(t);
        break;
    }
    getop();
    }
    return t;
}
char add(char t1)
{
    char t2,tmp[256],reg1[5],reg2[5];
    MatchChar("+");
    t2=term();
    t1=samesize(t1,t2,'c','a');
    t2=t1;
    if(t1=='d'||t1=='q')
    regname(reg1,'c','q');
    else if(t1=='b'||t1=='w')
    regname(reg1,'c','w');
    regname(reg2,'a',t2);
    sprintf(tmp,"pop %s",reg1);
    emitln(tmp);
    regname(reg1,'c',t1);
    sprintf(tmp,"add %s,%s",reg2,reg1);
    emitln(tmp);
    return t1;
}
char sub(char t1)
{
    char t2,tmp[256],reg1[5],reg2[5];
    MatchChar("-");
    t2=term();
    t1=samesize(t1,t2,'c','a');
    t2=t1;
    if(t1=='d'||t1=='q')
    regname(reg1,'c','q');
    else if(t1=='b'||t1=='w')
    regname(reg1,'c','w');
    regname(reg2,'a',t2);
    sprintf(tmp,"pop %s",reg1);
    emitln(tmp);
    regname(reg1,'c',t1);
    sprintf(tmp,"xchg %s,%s",reg2,reg1);
    emitln(tmp);
    sprintf(tmp,"sub %s,%s",reg2,reg1);
    emitln(tmp);
    return t2;
}
char expr()
{
    char t,reg1[5],tmp[256];
    t=term();
    while(isAdOp(op[0]))
    {
    if(t=='d'||t=='q')
    regname(reg1,'a','q');
    else if(t=='b'||t=='w')
    regname(reg1,'a','w');
    sprintf(tmp,"push %s",reg1);
    emitln(tmp);
    switch(op[0])
    {
        case '+':t=add(t);
        break;
        case '-':t=sub(t);
        break;
    }
    }
    return t;
}
char notexpr()
{
    char t,reg[5],tmp[20];
    if(l=='~')
    {
        getop();
        MatchChar("~");
        t=expr();
        regname(reg,'a',t);
        sprintf(tmp,"not %s",reg);
        emitln(tmp);
    }
    else
        t=expr();
        return t;
}
char and(char t1)
{
    char t2,tmp[256],reg1[5],reg2[5];
    MatchChar("&&");
    t2=notexpr();
    t1=samesize(t1,t2,'c','a');
    t2=t1;
    if(t1=='d'||t1=='q')
    regname(reg1,'c','q');
    else if(t1=='b'||t1=='w')
    regname(reg1,'c','w');
    regname(reg2,'a',t2);
    sprintf(tmp,"pop %s",reg1);
    emitln(tmp);
    regname(reg1,'c',t1);
    sprintf(tmp,"and %s,%s",reg2,reg1);
    emitln(tmp);
    return t1;
}
char or(char t1)
{
    char t2,tmp[256],reg1[5],reg2[5];
    MatchChar("||");
    t2=bitterm();
    t1=samesize(t1,t2,'c','a');
    t2=t1;
    if(t1=='d'||t1=='q')
    regname(reg1,'c','q');
    else if(t1=='b'||t1=='w')
    regname(reg1,'c','w');
    regname(reg2,'a',t2);
    sprintf(tmp,"pop %s",reg1);
    emitln(tmp);
    regname(reg1,'c',t1);
    sprintf(tmp,"or %s,%s",reg2,reg1);
    emitln(tmp);
    return t1;
}
char eor(char t1)
{
    char t2,tmp[256],reg1[5],reg2[5];
    MatchChar("^");
    t2=bitterm();
    t1=samesize(t1,t2,'c','a');
    t2=t1;
    if(t1=='d'||t1=='q')
    regname(reg1,'c','q');
    else if(t1=='b'||t1=='w')
    regname(reg1,'c','w');
    regname(reg2,'a',t2);
    sprintf(tmp,"pop %s",reg1);
    emitln(tmp);
    regname(reg1,'c',t1);
    sprintf(tmp,"eor %s,%s",reg2,reg1);
    emitln(tmp);
    return t1;
}

char bitterm()
{
    char t,reg1[5],tmp[256];
    t=notexpr();
    while(strcmp(op,"&&")==0)
    {
    if(t=='d'||t=='q')
    regname(reg1,'a','q');
    else if(t=='b'||t=='w')
    regname(reg1,'a','w');
    sprintf(tmp,"push %s",reg1);
    emitln(tmp);
    t=and(t);
    }
    return t;
}
char bitexpr()
{
    char t,reg1[5],tmp[256];
    t=bitterm();
    while((strcmp(op,"||")==0)||(strcmp(op,"^")==0))
    {
    if(t=='d'||t=='q')
    regname(reg1,'a','q');
    else if(t=='b'||t=='w')
    regname(reg1,'a','w');
    sprintf(tmp,"push %s",reg1);
    emitln(tmp);
    if(strcmp(op,"||"))
    t=or(t);
    if(strcmp(op,"^"))
    t=eor(t);
    }
    return t;
}
void equals(char t1)
{
    char t2,tmp[256],reg1[5],reg2[5];
    MatchChar("==");
    t2=bitexpr();
    t1=samesize(t1,t2,'c','a');
    t2=t1;
    if(t1=='d'||t1=='q')
    regname(reg1,'c','q');
    else if(t1=='b'||t1=='w')
    regname(reg1,'c','w');
    regname(reg2,'a',t2);
    sprintf(tmp,"pop %s",reg1);
    emitln(tmp);
    regname(reg1,'c',t1);
    sprintf(tmp,"cmp %s,%s",reg1,reg2);
    emitln(tmp);
    emitln("sete al");
}
void notequals(char t1)
{
    char t2,tmp[256],reg1[5],reg2[5];
    MatchChar("!=");
    t2=bitexpr();
    t1=samesize(t1,t2,'c','a');
    t2=t1;
    if(t1=='d'||t1=='q')
    regname(reg1,'c','q');
    else if(t1=='b'||t1=='w')
    regname(reg1,'c','w');
    regname(reg2,'a',t2);
    sprintf(tmp,"pop %s",reg1);
    emitln(tmp);
    regname(reg1,'c',t1);
    sprintf(tmp,"cmp %s,%s",reg1,reg2);
    emitln(tmp);
    emitln("setne al");
}
void lessequ(char t1)
{
    char t2,tmp[256],reg1[5],reg2[5];
    MatchChar("<=");
    t2=bitexpr();
    t1=samesize(t1,t2,'c','a');
    t2=t1;
    if(t1=='d'||t1=='q')
    regname(reg1,'c','q');
    else if(t1=='b'||t1=='w')
    regname(reg1,'c','w');
    regname(reg2,'a',t2);
    sprintf(tmp,"pop %s",reg1);
    emitln(tmp);
    regname(reg1,'c',t1);
    sprintf(tmp,"cmp %s,%s",reg1,reg2);
    emitln(tmp);
    emitln("setle al");
}
void lessthan(char t1)
{
    char t2,tmp[256],reg1[5],reg2[5];
    MatchChar("<");
    t2=bitexpr();
    t1=samesize(t1,t2,'c','a');
    t2=t1;
    if(t1=='d'||t1=='q')
    regname(reg1,'c','q');
    else if(t1=='b'||t1=='w')
    regname(reg1,'c','w');
    regname(reg2,'a',t2);
    sprintf(tmp,"pop %s",reg1);
    emitln(tmp);
    regname(reg1,'c',t1);
    sprintf(tmp,"cmp %s,%s",reg1,reg2);
    emitln(tmp);
    emitln("setl al");
}
void greatequ(char t1)
{
    char t2,tmp[256],reg1[5],reg2[5];
    MatchChar(">=");
    t2=bitexpr();
    t1=samesize(t1,t2,'c','a');
    t2=t1;
    if(t1=='d'||t1=='q')
    regname(reg1,'c','q');
    else if(t1=='b'||t1=='w')
    regname(reg1,'c','w');
    regname(reg2,'a',t2);
    sprintf(tmp,"pop %s",reg1);
    emitln(tmp);
    regname(reg1,'c',t1);
    sprintf(tmp,"cmp %s,%s",reg1,reg2);
    emitln(tmp);
    emitln("setge al");
}
void greatthan(char t1)
{
    char t2,tmp[256],reg1[5],reg2[5];
    MatchChar(">");
    t2=bitexpr();
    t1=samesize(t1,t2,'c','a');
    t2=t1;
    if(t1=='d'||t1=='q')
    regname(reg1,'c','q');
    else if(t1=='b'||t1=='w')
    regname(reg1,'c','w');
    regname(reg2,'a',t2);
    sprintf(tmp,"pop %s",reg1);
    emitln(tmp);
    regname(reg1,'c',t1);
    sprintf(tmp,"cmp %s,%s",reg1,reg2);
    emitln(tmp);
    emitln("setg al");
}
char rel()
{
    char t1,reg1[5],tmp[256];
    t1=bitexpr();
    if(isReOp(op))
    {
        if(t1=='d'||t1=='q')
        regname(reg1,'a','q');
        else if(t1=='b'||t1=='w')
        regname(reg1,'a','w');
        sprintf(tmp,"push %s",reg1);
        emitln(tmp);
        switch(op[0])
        {
            case '=':if(op[1]=='=')
                        equals(t1);
                        break;
            case '!': if(op[1]=='=')
                        notequals(t1);
                        break;
            case '<': if(op[1]=='=')
                        lessequ(t1);
                        else
                        lessthan(t1);
                        break;
            case '>': if(op[1]=='=')
                        greatequ(t1);
                        else
                        greatthan(t1);
                        break;
        }
        sprintf(tmp,"test al,al",reg1,reg1);
        emitln(tmp);
        return 'b';
    }
    else
        return t1;
}

char boolfactor()
{
    short b;
    char t;
    b=getbool();
    if(b==1)
    {
    emitln("mov rax,1");
    getop();
    t='b';
    }
    else if(b==0)
    {
    emitln("mov rax,0");
    getop();
    t='b';
    }
    else
    t=rel();
    return t;
}

char notfactor()
{
    char t;
    if(l=='!')
    {
        getop();
        MatchChar("!");
        t=boolfactor();
        emitln("not al");
    }
    else
        t=boolfactor();
        return t;
}

char booland()
{
    char t;
    MatchChar("&");
    t=notfactor();
    emitln("pop cx");
    emitln("and al,cl");
    return t;
}
char boolor()
{
    char t;
    MatchChar("|");
    t=boolterm();
    emitln("pop cx");
    emitln("or al,cl");
    return t;
}

char booleor()
{
    char t;
    MatchChar("`");
    t=boolterm();
    emitln("pop cx");
    emitln("eor al,cl");
    return t;
}
char boolterm()
{
    char t;
    t=notfactor();
    while(op[0]=='&')
    {
    emitln("push ax");
    t=booland();
    }
    return t;
}

char boolexpr()
{
    char t;
    t=boolterm();
    while(op[0]=='|'||op[0]=='`')
    {
    emitln("push ax");
    switch(op[0])
    {
        case '|':t=boolor();
        break;
        case '`':t=booleor();
        break;
    }
    }
    return t;
}
void assign(void *t,void *lt)
{
    char tmp1[256],tmp2[296],reg1[5],reg2[5],c,st,dt;
    struct identifier* type;
    struct localident* ltype;
    if(t!=NULL)
    {
    type=(struct identifier*) t;
    sprintf(tmp1,"[%s]",type->id);
    dt=type->idt;
    }
    else if(lt!=NULL)
    {
    ltype=(struct localident*)lt;
    dt=ltype->idt;
    if(ltype->nb>8)
    {
        strcpy(reg2,"r10");
        sprintf(tmp2,"mov %s,[rbp+%d]",reg2,ltype->nb);
        emitln(tmp2);
        sprintf(tmp1,"[%s]",reg2);
    }
    else
        sprintf(tmp1,"[rbp%d]",ltype->nb);
    }
    getop();
    MatchChar("=");
    st=boolexpr();
    convert(st,dt,'a');
    regname(reg1,'a',dt);
    sprintf(tmp2,"mov %s,%s",tmp1,reg1);
    emitln(tmp2);
}
char* NewLabel()
{
    char *Label;
    Label=malloc(5*sizeof(char));
    ln++;
    sprintf(Label,"L%d",ln);
    return Label;
}

void freeLabel(char* L)
{
    free(L);
}
void PostLabel(char* l)
{
    fprintf(outfp,"%s:",l);
}
void condition()
{
    boolexpr();
}
void ifst(void* lt,char* Label)
{
    char tmp[256];
    char *Label1,*Label2;
    Match("if");
    MatchSym('(');
    condition();
    MatchSym(')');
    Label1=NewLabel();
    Label2=NewLabel();
    sprintf(tmp,"je %s",Label2);
    emitln(tmp);
    getword();
    Match("begin");
    block(lt,Label);
    Match("end");
    getword();
    if(strcmp(token,"else")==0)
    {
        sprintf(tmp,"jmp %s",Label1);
        emitln(tmp);
        PostLabel(Label2);
        freeLabel(Label2);
        elsest(lt,Label);
        getword();
        PostLabel(Label1);
        freeLabel(Label1);
    }
    else
    {
        PostLabel(Label2);
        freeLabel(Label2);
    }
}
void elsest(void* lt,char* Label)
{
    Match("else");
    getword();
    Match("begin");
    block(lt,Label);
    Match("end");
}
void whilest(void* lt)
{
    char tmp[256];
    char *Label1, *Label2;
    Match("while");
    Label2=NewLabel();
    PostLabel(Label2);
    MatchSym('(');
    condition();
    MatchSym(')');
    Label1=NewLabel();
    sprintf(tmp,"je %s",Label1);
    emitln(tmp);
    getword();
    Match("begin");
    block(lt,Label1);
    Match("end");
    sprintf(tmp,"jmp %s",Label2);
    emitln(tmp);
    PostLabel(Label1);
    freeLabel(Label1);
    freeLabel(Label2);
}
void* loc_decl(void *lt,void *ct)
{
    short bytes;
    char tmp[256],t;
    bytes=-(((struct identifier*)lt)->nbytes);
    if(strcmp(token,"byte")==0)
        t='b';
    else if(strcmp(token,"short")==0)
        t='w';
    else if(strcmp(token,"int")==0)
        t='d';
    else if(strcmp(token,"long")==0)
        t='q';
    do
    {
        getword();
        ct=creatlid(t,lt,ct);
        if(t=='b')
        bytes--;
        else if(t=='w')
        bytes-=2;
        else if(t=='d')
        bytes-=4;
        else if(t=='q')
        bytes-=8;
        ((struct localident*)ct)->nb=bytes;
        ((struct identifier*)lt)->nbytes=bytes;
    }while(MatchSym(','));
    ((struct identifier*)lt)->nbytes=-(((struct identifier*)lt)->nbytes);
    return ct;
}

void proc_call(void* proc)
{
    char tmp[256],reg1[5],t;
    void* ct;
    short bytes=0,na=0,pn=0;
    MatchSym('(');
    while(l!=')')
    {
    if(IsAlph(l)||l=='_')
    getword();
    ct=findtype();
    if(ct!=NULL)
    {
    t=((struct identifier*)ct)->idt;
    pn++;
    regname(reg1,'a',t);
    sprintf(tmp,"lea %s,[%s]",reg1,token);
    emitln(tmp);
    }
    else
    {
    ct=findpvtype(procname,&na);
    bytes=((struct localident*)ct)->nb;
    t=((struct identifier*)ct)->idt;
    if(bytes>8)
    {
        sprintf(tmp,"mov rax,[rbp+%d]",bytes);
        emitln(tmp);
    }
    else
    {
        sprintf(tmp,"lea rax,[rbp%d]",bytes);
        emitln(tmp);
    }
    }
    if(l==',')
    MatchSym(',');
    sprintf(tmp,"push rax");
    emitln(tmp);
    }
    MatchSym(')');
    sprintf(tmp,"call %s",((struct identifier*)proc)->id);
    emitln(tmp);
}

void assignorproc(void* lt)
{
    void *type,*ltype;
    short na;
    type=findtype();
    if(type==NULL)
    {
        ltype=findpvtype(lt,&na);
        if(ltype!=NULL)
        assign(type,ltype);

    }
    else
    {
    if((((struct identifier*)type)->idt)=='p')
        proc_call(type);
    else
        assign(type,NULL);
    }
}
void breakst(char* Label)
{
    char tmp[25];
    Match("break");
    sprintf(tmp,"jmp %s",Label);
    emitln(tmp);
}
void forst(void* lt)
{
    char tmp[25];
    char *Label1, *Label2,*Label3,*Label4;
    Match("for");
    MatchSym('(');
    getword();
    assignorproc(lt);
    MatchSym(';');
    Label1=NewLabel();
    PostLabel(Label1);
    boolexpr();
    Label3=NewLabel();
    sprintf(tmp,"je %s",Label3);
    emitln(tmp);
    MatchSym(';');
    Label2=NewLabel();
    sprintf(tmp,"jmp %s",Label2);
    emitln(tmp);
    getword();
    Label4=NewLabel();
    PostLabel(Label4);
    assignorproc(lt);
    sprintf(tmp,"jmp %s",Label1);
    emitln(tmp);
    MatchSym(')');
    getword();
    Match("begin");
    PostLabel(Label2);
    block(lt,Label3);
    sprintf(tmp,"jmp %s",Label4);
    emitln(tmp);
    PostLabel(Label3);
    Match("end");
}
void pblock(void* lt,void *ct)
{
    getword();
    while((strcmp(token,"end"))!=0)
    {
        if((strcmp(token,"if"))==0)
        ifst(lt,NULL);
        else if((strcmp(token,"while"))==0)
        {
        whilest(lt);
        getword();
        }
        else if((strcmp(token,"for"))==0)
        {
        forst(lt);
        getword();
        }
        else if((strcmp(token,"byte"))==0||(strcmp(token,"short"))==0||(strcmp(token,"int"))==0||(strcmp(token,"long")==0))
        {
        ct=loc_decl(lt,ct);
        getword();
        }
        else
        {
        assignorproc(lt);
        getword();
        }
    }
}
void block(void* lt,char* Label)
{
    getword();
    while((strcmp(token,"end"))!=0)
    {
        if((strcmp(token,"if"))==0)
        ifst(lt,Label);
        else if((strcmp(token,"while"))==0)
        {
        whilest(lt);
        getword();
        }
        else if((strcmp(token,"for"))==0)
        {
        forst(lt);
        getword();
        }
        else if((strcmp(token,"break"))==0)
        {
        breakst(Label);
        getword();
        }
        else
        {
        assignorproc(lt);
        getword();
        }
    }
}
void* param_decl(void *lt)
{
    int nb;
    char t;
    void *c;
    short bytes=8;
    struct localident* ct;
    do
    {
    getword();
    if(strcmp(token,"byte")==0)
        t='b';
    else if(strcmp(token,"short")==0)
        t='w';
    else if(strcmp(token,"int")==0)
        t='d';
    else if(strcmp(token,"long")==0)
        t='q';
    getword();
    c=creatlid(t,lt,c);
        ((struct identifier*)lt)->pbytes=bytes;
        bytes+=8;
        if(l==',')
            MatchSym(',');
        else if(l!=')')
        {
            expected(", or )");
            break;
        }
    }while(l!=')');
    ct=((struct identifier*)lt)->locltblst;
    while(ct!=NULL)
    {
        ct->nb=bytes;
        bytes-=8;
        ct=ct->next;
    }
    return c;
}
void proc()
{
    char tmp[25];
    void *locltbl;
    void *ct;
    short bytes=0;
    long offset1=0,offset2=0;
    Match("proc");
    getword();
    PostLabel(token);
    emitln("push rbp");
    emitln("mov rbp,rsp");
    emit("sub rsp,");
    offset1=ftell(outfp);
    emit("      ");
    locltbl=createid('p');
    procname=locltbl;
    MatchSym('(');
    ct=param_decl(locltbl);
    MatchSym(')');
    pblock(locltbl,ct);
    offset2=ftell(outfp);
    fseek(outfp,offset1,0);
    sprintf(tmp,"%d\n",((struct identifier*)locltbl)->nbytes);
    fprintf(outfp,tmp);
    fseek(outfp,offset2,0);
    sprintf(tmp,"add rsp,%d",((struct identifier*)locltbl)->nbytes);
    emitln(tmp);
    emitln("mov rsp,rbp");
    emitln("pop rbp");
    emitln("ret");
    Match("end");
}
void MainProg()
{
    void *locltbl;
    void *ct=NULL;
    char tmp[25];
    long offset1=0,offset2=0;
    Match("program");
    PostLabel("start");
    emitln("push rbp");
    emitln("mov rbp,rsp");
    emit("sub rsp,");
    offset1=ftell(outfp);
    emit("      ");
    locltbl=createid('p');
    procname=locltbl;
    pblock(locltbl,ct);
    offset2=ftell(outfp);
    fseek(outfp,offset1,0);
    sprintf(tmp,"%d\n",((struct identifier*)locltbl)->nbytes);
    fprintf(outfp,tmp);
    fseek(outfp,offset2,0);
    sprintf(tmp,"add rsp,%d",((struct identifier*)locltbl)->nbytes);
    emitln(tmp);
    emitln("mov rsp,rbp");
    emitln("pop rbp");
    emitln("ret");
    Match("end");
}

void var_decl()
{
    char tmp[256];
    char t;
    sprintf(tmp,"section '.data' data readable writeable",token,t);
    emitln(tmp);
    if(strcmp(token,"byte")==0)
        t='b';
    else if(strcmp(token,"short")==0)
        t='w';
    else if(strcmp(token,"int")==0)
        t='d';
    else if(strcmp(token,"long")==0)
        t='q';
    do
    {
        getword();
        sprintf(tmp,"%s\td%c\t?",token,t);
        createid(t);
        emitln(tmp);
    }while(MatchSym(','));
}
void program()
{
    readchar();
    emitln("format PE64");
    emitln("entry start");
    while(l!=EOF)
    {
        getword();
        if((strcmp(token,"proc")==0)||(strcmp(token,"program")==0))
        {
        if(codesection==0)
        {
        emitln("section '.text' code readable executable");
        codesection=1;
        }
        if(strcmp(token,"proc")==0)
            proc();
        else if(strcmp(token,"program")==0)
        {
            MainProg();
            break;
        }
        }
        else
            var_decl();
    }
}

int main(int argc,char *argv[])
{
    openfile(argv[1]);
    writefile(argv[2]);
    program();
    closefile();
    delid();
    return 0;
}
