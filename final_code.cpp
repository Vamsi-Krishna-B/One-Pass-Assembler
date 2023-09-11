#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
using namespace std;
ofstream objP("output.txt");
string ST[10];
int SI;
int xy = 0, loc = 0;
int locctr;
int locctr2;
int staddr;
int length;
int len_TR = 0;
string opd;
string name;
int conv(string myString) // sends hexadecimal equivalent
{
    int x;
    stringstream ss(myString);
    ss >> hex >> x;
    return x;
}
static int ocount = 0; // count of no.of opcodes
class OpTab
{

public:
    string opcode, hexcode;
    friend string Hexcode(string s);
    friend int OCheck(string s);

} OPTAB[30];

string Hexcode(string s)
{
    string x = "00";
    for (int i = 0; i < ocount; i++)
    {
        if (OPTAB[i].opcode == s)
        {
            x = OPTAB[i].hexcode;
            break;
        }
    }
    return x;
}
int OCheck(string s)
{
    if (s == "START" || s == "END")
        return 0;
    else if (s == "WORD")
        return 1;
    else if (s == "BYTE")
        return 2;
    else if (s == "RESB")
        return 3;
    else if (s == "RESW")
        return 4;
    else
        return -1;
}

static int scount = 0;
class SymTab
{

public:
    int value = -2;
    SymTab *next;
    string label;
    int addr = 0;
    int flag = 0;
    friend void Check(string s);
    friend void New_TR(string s, int addr, int fl);
    friend int return_flag(string s);
    friend int Value_At(string s);
    SymTab()
    {
        this->next = NULL;
    }

} SYMTAB[30];

void Check(string s) // to check and insert the value of symbols in symtab
{
    int c = 0;
    // cout << s << " in check - " << scount <<" "<< endl;
    if (scount != 0)
    {
        for (int i = 0; i < scount; i++)
        {

            if (SYMTAB[i].label == s)
            {
                if (SYMTAB[i].value == 0xfffffffe) // loop-1
                {
                    SYMTAB[i].value = locctr;
                    SYMTAB[i].flag = -1;
                    SymTab *temp;
                    // cout << SYMTAB[i].label << " " << SYMTAB[i].value << " " << hex << locctr << "loop-1" << endl;
                    temp = &SYMTAB[i];
                    while (temp->next != NULL)
                    {
                        temp = temp->next;
                        temp->value = locctr;
                        temp->label = s;
                    }
                    break;
                }
                else if (SYMTAB[i].value != 0xfffffffe)
                {
                    break;
                }
            }
            else
            {
                c++;
            }
        }
        if (c == scount)
        {
            SYMTAB[scount].label = s;
            SYMTAB[scount].value = locctr;
            // cout << SYMTAB[scount].label << " " << SYMTAB[scount].value << " " << hex << locctr << "loop-2"
            //  << "scount " << scount << endl;
            scount++;
        }
    }
    else // loop -3
    {
        SYMTAB[scount].label = s;
        SYMTAB[scount].value = locctr;
        // cout << SYMTAB[scount].label << " " << SYMTAB[scount].value << " " << hex << locctr << "loop-3"
        //  << "scount " << scount << endl;
        scount++;
    }
}

int Value_At(string s)
{
    int c = 0;
    int x = 0;
    for (int i = 0; i < scount; i++)
    {
        if (SYMTAB[i].label == s)
        {
            // cout << "in va;ue loop-1 " << SYMTAB[i].label << " sc->" << scount << "scount " << scount << endl;

            if (SYMTAB[i].value == -2)
            // if (SYMTAB[i].value == 0xfffffffe)
            {
                if (scount == c)
                {
                    // cout << "\n\n *** \n\n";
                }
                // cout << "in value lop=2" << "\nlocctr = "<<locctr<<endl;
                SymTab *temp;
                temp = &SYMTAB[i];
                SymTab *temp1;
                temp1 = &SYMTAB[i];
                SYMTAB[i].flag = -1;
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                temp->next = new SymTab;
                temp = temp->next;
                temp->addr = locctr + 0x1 * 1;
                // cout<<"aadr = "<< temp->addr;
                temp->flag = -1;
                temp->label = s;
                temp->value = -2;
                temp->next = NULL;
                // cout<<SYMTAB[i].addr<<"->"<<temp1->next->addr<<"->"<<temp1->next->next->addr;
            }
            else if (SYMTAB[i].value != -2)
            // else if (SYMTAB[i].value != 0xfffffffe)
            {
                // cout << " in return value<" << endl;
                x = SYMTAB[i].value;
                break;
            }
        }
        else
        {
            c++;
        }
    }
    if (c == scount)
    {

        // cout << "in value " << scount << " " << s << " " << hex << SYMTAB[scount].value << hex << locctr << endl;
        SYMTAB[scount].label = s;
        SYMTAB[scount].value = -2;
        SYMTAB[scount].flag = -1;
        SYMTAB[scount].addr = 0;
        SymTab *temp;
        temp = &SYMTAB[scount];
        temp->next = new SymTab;
        temp = temp->next;
        SymTab *temp1;
        temp1 = &SYMTAB[scount];
        // temp1->next = new SymTab;
        temp->addr = locctr + 0x1 * 1;
        // cout << "flag=" << SYMTAB[scount].flag;
        // cout << SYMTAB[scount].label << "->addr :" << SYMTAB[scount].addr << " next->addr= " << temp1->next->addr << endl;
        temp->flag = -1;
        temp->label = s;
        temp->value = -2;
        temp->next = NULL;
        scount++;
    }
    return x;
}
void traversal(SymTab *p)
{
    cout << p->label << " ->";
    SymTab *temp;
    temp = p;
    while (p != NULL)
    {
        cout << temp->addr << " "
             << "locctr = " << temp->value << "->";
        temp = temp->next;
    }
    cout << endl;
}

string f(int x)
{
    stringstream ss;
    ss << hex << x;
    string t;
    ss >> t;
    int p = t.length();
    string c = "";
    char ch[6 - p];
    for (int i = 0; i < 6 - p; i++)
    {
        ch[i] = '0';
        c = c + ch[i];
    }
    //  ss<<hex<<x;
    c = c + t;
    return c;
}
int return_flag(string s)
{
    int k = 0;
    for (int i = 0; i < scount; i++)
    {
        if (SYMTAB[i].label == s)
        {
            k = SYMTAB[i].flag;
            break;
        }
    }
    return k;
}
string f2(string st)
{
    int p = st.length();
    char ch[6];
    for (int i = 0; i < 6 - p; i++)
    {
        ch[i] = ' ';
        st = st + ch[i];
    }
    return st;
}
void New_TR(string s, int addr, int fl)
{
    // filesystem :: path out =filesystem :: current_path();
    // out+="\\output.txt";
    if (fl == 0)
    {
        objP << "\nT^" << f(addr) << "^";
        len_TR = 0;
        SI = 0;
        ST[SI] = s;
        len_TR += s.length() / 2;
        SI++;
    }
    else if (fl == 1)
    {
        objP << "\nE^" << f(loc);
        objP.close();
    }
    else if (fl == -1)
    {
        for (int i = 0; i < scount; i++)
        {
            if (SYMTAB[i].value == addr)
            {
                SymTab *temp;
                temp = &SYMTAB[i];
                temp = temp->next;
                while (temp != NULL)
                {
                    objP << "\nT^" << f(temp->addr) << "^02^" << addr;
                    temp = temp->next;
                }
            }
        }
        // cout<<endl;
        New_TR(s, addr, 0);
    }
    else if (fl == -2)
    {
        New_TR(s, addr, 0);
    }
}
void Add_To_TR(string st, int add, int flag)
{
    int x = st.length() / 2;
    // cout<<"st->"<<staddr<<endl;
    if ((len_TR + x) > 30 || len_TR == 0 || st == "" || flag == -1 || add == loc)
    {
        int z = flag;
        if (st == "")
        {
            z = 1;
        }
        if (add == loc)
        {
            z = -2;
        }
        objP << hex << len_TR;
        for (int i = 0; i < SI; i++)
        {
            objP << "^" << ST[i];
        }
        New_TR(st, add, z);
    }
    else
    {
        ST[SI] = st;
        len_TR += x;
        SI++;
    }
}

int main()
{

    filesystem ::path o = filesystem ::current_path();
    o += "\\output.txt";
    filesystem ::path in = filesystem ::current_path();
    in += "\\input.txt";
    filesystem ::path opt = filesystem ::current_path();
    opt += "\\optab.txt";
    filesystem ::path sym = filesystem ::current_path();
    sym += "\\symtab.txt";
    filesystem ::path Of = filesystem ::current_path();
    Of += "\\out_prog.txt";
    filesystem ::path objp = filesystem ::current_path();
    objp += "\\Object_Prog.txt";
    ifstream input; // to read input file
    ifstream optab; // to read optable
    ofstream Object;
    ofstream symtab; // to enter values to symtab
    ofstream Ofile;  // to get the program with location counters assigned
    Object.open(objp);
    input.open(in);
    optab.open(opt);
    symtab.open("symtab.txt");
    Ofile.open("out_prog.txt");

    // //getting the optab ready
    string lt;
    string s1, s2;
    char ch = ' ';

    while (optab.eof() == 0)
    {
        getline(optab, lt);
        stringstream S(lt);
        getline(S, s1, ch);
        getline(S, s2, ch);
        // cout<<s1<<" "<<s2<<endl;
        OPTAB[ocount].opcode = s1;
        OPTAB[ocount].hexcode = s2;
        // cout << OPTAB[ocount].opcode <<" ";
        // cout << OPTAB[ocount].hexcode <<" - "<<ocount<< endl;
        ocount++;
    }
    optab.close();

    string line;
    string st1, st2, st3; // three differenet columns in programs so one string to read each column
    while (opd != "END")
    {
        getline(input, line);
        stringstream ss(line);
        char ch = ' ';
        getline(ss, st1, ch);
        getline(ss, st2, ch);
        getline(ss, st3, ch);

        stringstream SS1;
        string h;
        if (st1 != ".")
        {
            if (st1 != "" && st2 != "" && st3 != "") // this identifies the three columns are filled
            {
                opd = st2;
                Check(st1);
                int l = return_flag(st1);
                // cout << "\n"
                //      << st1 << "->" << l << endl;

                if (st2 == "START")
                {
                    locctr = 0x1 * conv(st3);
                    staddr = locctr;
                    locctr2 = locctr;
                    name = st1;
                    Ofile << hex << locctr << " " << st1 << " " << st2 << " " << st3 << endl;
                    objP << "H^" << f2(name) << "^" << f(locctr2);
                }

                else if (OCheck(st2) == 1)
                {
                    int d;
                    stringstream ss(st3);
                    ss >> d;
                    Ofile << hex << locctr << " " << st1 << " " << st2 << " " << st3 << " " << f(d) << endl;
                    Add_To_TR(f(d), locctr, l);
                    locctr += 0x3;
                }
                else if (OCheck(st2) == 2)
                {
                    Ofile << hex << locctr << " " << st1 << " " << st2 << " " << st3 << " ";
                    if (st3[0] == 'C' || st3[0] == 'c')
                    {
                        int si = st3.length() - 3;
                        for (int i = 2; i < si + 2; i++)
                        {
                            Ofile << hex << (int)st3[i];
                            SS1 << hex << (int)st3[i];
                        }
                        SS1 >> h;
                        Add_To_TR(h, locctr, l);
                        locctr += 0x1 * si;
                    }
                    else if (st3[0] == 'X' || st3[0] == 'x')
                    {
                        int si = st3.length();
                        for (int i = 2; i < si - 1; i++)
                        {
                            Ofile << st3[i];
                            SS1 << st3[i];
                        }
                        SS1 >> h;
                        Add_To_TR(h, locctr, l);
                        locctr += 1;
                    }
                    else
                    {
                        int d = conv(st3);
                        Ofile << hex << locctr << " " << st1 << " " << st2 << " " << st3 << " " << f(d) << endl;

                        int r = st3.length();
                        char t[r];
                        for (int i = 0; i < r; i++)
                        {
                            t[i] = st3[i];
                        }
                        SS1 << f(d);
                        SS1 >> h;
                        Add_To_TR(h, locctr, l);
                        locctr += (0x3 * (atoi(t)));
                        Ofile << hex << locctr;
                    }
                    Ofile << endl;
                }
                else if (OCheck(st2) == 3)
                {

                    int d = conv(st3);
                    Ofile << locctr << " " << st1 << " " << st2 << " " << st3 << " " << endl;

                    int r = st3.length();
                    char t[r];
                    for (int i = 0; i < r; i++)
                    {
                        t[i] = st3[i];
                    }
                    locctr += (0x1 * (atoi(t)));
                }
                else if (OCheck(st2) == 4)
                {
                    int d = conv(st3);
                    Ofile << hex << locctr << " " << st1 << " " << st2 << " " << st3 << " " << endl;
                    int r = st3.length();
                    char t[r];
                    for (int i = 0; i < r; i++)
                    {
                        t[i] = st3[i];
                    }
                    locctr += (0x3 * (atoi(t)));
                }
                else if (OCheck(st2) == -1)
                {

                    if (xy == 0)
                    {
                        loc = locctr;
                    }
                    xy++;
                    int x = Value_At(st3);
                    if (x != 0)
                    {
                        Ofile << hex << locctr << " " << st1 << " " << st2 << " " << st3 << " " << Hexcode(st2) << hex << x << endl;
                        SS1 << Hexcode(st2) << hex << x;
                        SS1 >> h;
                        Add_To_TR(h, locctr, l);
                    }
                    else if (x == 0)
                    {
                        Ofile << hex << locctr << " " << st1 << " " << st2 << " " << st3 << " " << Hexcode(st2) << "0000" << endl;
                        SS1 << Hexcode(st2) << "0000";
                        SS1 >> h;
                        Add_To_TR(h, locctr, l);
                    }

                    locctr += 0x3;
                }
            }

            else if (st1 != "" && st2 != "" && st3 == "")
            {
                opd = st1;
                if (OCheck(st1) == -1)
                {
                    int t = st2.length();
                    if (st2[t - 2] == ',')
                    {
                        string s;
                        char ch = ',';
                        stringstream ss(st2);
                        getline(ss, s, ch);
                        int x = Value_At(s);
                        if (x != 0)
                        {
                            Ofile << hex << locctr << " " << st1 << " " << st2 << " " << Hexcode(st1) << hex << x + 0x8000 * 1 << endl;
                            SS1 << Hexcode(st1) << hex << x + 0x8000 * 1;
                            SS1 >> h;
                            Add_To_TR(h, locctr, 0);
                        }
                        else if (x == 0)
                        {
                            Ofile << hex << locctr << " " << st1 << " " << st2 << " " << Hexcode(st1) << "0000" << endl;
                            SS1 << Hexcode(st1) << "0000";
                            SS1 >> h;
                            Add_To_TR(h, locctr, 0);
                        }
                        locctr += 0x1 * 3;
                    }
                    else
                    {
                        int x = Value_At(st2);
                        if (x != 0)
                        {
                            Ofile << hex << locctr << " " << st1 << " " << st2 << " " << Hexcode(st1) << hex << x << endl;
                            SS1 << Hexcode(st1) << hex << x;
                            SS1 >> h;
                            Add_To_TR(h, locctr, 0);
                        }
                        else if (x == 0)
                        {
                            Ofile << hex << locctr << " " << st1 << " " << st2 << " " << Hexcode(st1) << "0000" << endl;
                            SS1 << Hexcode(st1) << "0000";
                            SS1 >> h;
                            Add_To_TR(h, locctr, 0);
                        }
                        locctr += 0x1 * 3;
                    }
                }
                else if (OCheck(st1) == 0)
                {
                    Ofile << st1 << " " << st2 << " ";
                    Add_To_TR("", locctr, 0);
                }
            }
            else if (st1 != "" && st2 == "" && st3 == "")
            {
                opd = st1;
                Ofile << hex << locctr << " " << st1 << " " << st2 << " " << Hexcode(st1) << "0000" << endl;
                SS1 << Hexcode(st1) << "0000";
                SS1 >> h;
                Add_To_TR(h, locctr, 0);
                locctr += 0x1 * 3;
            }
        }
        else
        {
            Ofile << st1 << endl;
            // Add_To_TR(st1,locctr);
        }

        st1.clear();
        st2.clear();
        st3.clear();
    }
    // cout << "\nE^" << hex << loc << endl;
    cout << "scount :" << scount << endl;
    for (int i = 0; i < scount; i++)
    {
        symtab << SYMTAB[i].label << " " << hex << SYMTAB[i].value << "  flag- " << SYMTAB[i].flag << " i- > " << i << endl;
    }
    cout << "Length of program is " << hex << locctr - staddr << endl;

    ifstream out;
    out.open(o);
    int u = 0;
    length = locctr - staddr;
    while (out.eof() == 0)
    {
            string t;
            getline(out, t);
        if (u == 0)
        {
            Object << "H^" << f2(name) << "^" << f(locctr2) << "^" << f(length) << endl;
        }
        else
        {
            Object << t << endl;
        }
        u++;
    }
    optab.close();
    out.close();
    Object.close();
    input.close();
    symtab.close();
    Ofile.close();

    return 0;
}