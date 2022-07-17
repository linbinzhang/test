#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
 
using namespace std;
 
/***
 *
 * Output format:
 *<symbol>,<MaxTimeGap>,<Volume>,<WeightedAveragePrice>,<MaxPrice>
 *
 *Output filename: output.csv  
 *
 *
***/


int main()
{

size_t MaxTimeGap, TotalVolume, WAP, MaxPrice;
size_t Dt, Time, Volume, Price, SumPrice; 
size_t     Time1, Volume1, Price1;

vector<pair<string, int>> vp;

vector<vector<string>> content;
vector<vector<string>> contentSorted;
vector<string> row;
string line, word;

stringstream ss;

string fname;
cout<<"Enter the file name: ";
cin>>fname;
 
fstream file (fname, ios::in);

/* read data   */
if(file.is_open())
{
  while(getline(file, line))
  {
        row.clear();
        stringstream str(line);
 
        while(getline(str, word, ','))
              row.push_back(word);
        content.push_back(row);
  }
} else {
  cout<<"Could not open the file\n";
}

file.close();

/*  sort stock's name before compute the results */

for ( int i = 0; i < content.size();i++)
{
      vp.push_back(make_pair(content[i][1], i));
} 

sort(vp.begin(), vp.end());

for (int i=0;i < vp.size();i++)
{
   contentSorted.push_back(content[vp[i].second]);
}

  /*  open output csv file */ 
  file.open("output.csv", ios::out); 

  /* start the work */
  line = contentSorted[0][1];

  ss << contentSorted[0][0];
  ss >>Time;
  ss.clear();

  ss << contentSorted[0][2];
  ss >> Volume;
  ss.clear();

  ss << contentSorted[0][3];
  ss >> Price; 
  ss.clear();  
 
  MaxPrice = Price;
  MaxTimeGap = 0;
  TotalVolume = Volume; 
  SumPrice = Volume * Price;  
   
  /*   loop over all stock */ 

  for (int i = 1; i < contentSorted.size();i++)
  {
     /* read one line data */
     word = contentSorted[i][1];
     ss << contentSorted[i][0];
     ss >>Time1;
     ss.clear();
     ss << contentSorted[i][2];
     ss >> Volume1;
     ss.clear();
     ss << contentSorted[i][3];
     ss >> Price1;
     ss.clear(); 
     
     if( word == line) {
       /*** if the stock has the same name as previous one ***/

       /* Estimate maximum time gap */
       Dt = Time1 - Time;
       if(Volume1 == 1) Dt = 0.0;
       if( Dt >= MaxTimeGap) {
           MaxTimeGap = Dt; 
       }

       /* Volumes */
       TotalVolume += Volume1;

       /* Maximum price */
       if( Price1 >= MaxPrice){
           MaxPrice = Price1;
       }
   
       SumPrice += Volume1 * Price1; 
       Time = Time1;
       
     } else {
     /*** hit difference stock, output the results and reset for this stock info ***/ 
       /*  compute average  */
       WAP = SumPrice/TotalVolume;

       /*  output results */
       file << line;
       file<<",";
       file << to_string(MaxTimeGap);
       file<<",";
       file << to_string(TotalVolume);
       file<<",";
       file << to_string(WAP);
       file<<",";
       file << to_string(MaxPrice);
       file<<"\n";
  
       /* reset every for next stock */ 
       MaxPrice = Price1;
       MaxTimeGap = 0;
       line = word;
       Time = Time1;
       TotalVolume = Volume1;
       SumPrice = Volume1 * Price1;
     }

  }

  /*  compute average  */
  WAP = SumPrice/TotalVolume;

  /* output results */
  file << word;
  file<<",";
  file << to_string(MaxTimeGap);
  file<<",";
  file << to_string(TotalVolume);
  file<<",";
  file << to_string(WAP);
  file<<",";
  file << to_string(MaxPrice);
  file<<"\n";

  file.close();

return 0;

}
