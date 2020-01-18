
// Written by : Kunal D. Patil
// Date: 26 March 2013
// Place: CoNe Lab, IISc

#include <stdio.h>
#include <string.h>
#define Num_keywords 14
#define Event_keywords 3
#define _length 110
#define _JUNK 101
#define _TITLE 1
#define _ELTYPE 2
#define _NODES 3
#define _ELEMENTS 4
#define _MATERIAL 5
#define _EVENTS 6
#define _MAXITERATIONS 7
#define _SPDISP 8
#define _NDTEMPER 9
#define _SPFPOT 10
#define _STEP 11
#define _CFORCE 12
#define _PRESS 13
#define _BODY 14



int main (int argc, char **argv)
 {     
        FILE *file0, *file1, *file2, *file3a, *file3b, *file3c, *file3d, *file4, *file5, *file6,*file7,*file8,*filec1,*filec2,*filec3,*filec4,*filec5,*fin,*dens,*file9,*file10,*file11,*file12;
        FILE *type1,*type2,*type3,*type4,*tp1,*tp2,*tp3,*tp4,*dens_data,*timedata;
	char string[Num_keywords][14] = {"*TITLE","*ELTYPE","*NODES","*ELEMENTS","*MATERIAL","*EVENT","*MAXITERATIONS","*SPDISP","*NDTEMPER","*SPFPOT","*STEP","*CFORCE","*PRESS","*BODY"};
        char line[_length],linedump[_length];
        char eltype[6],eltype_w[6];
	char disp_type[3];
        char eltype_hyb[6],eltype_hyb_w[6];
        char event_string[Event_keywords][5]={"INCR","MAXI","TOLE"};
        char dummy_string[5][_length];
	char Pot[]={"VOLT"};	
	char mat_prop[5];
	char NSRL_var[4]={0,0,0,0},NKTP_var[4]={0,0,0,0},NORDR_var[4]={0,0,0,0};
	char load_def[3];

        int status = _JUNK, NSRL, NKTP, NORDR;
        int n_EleType = 0, n_EleType_hyb = 0,n_Material = 0, n_Boundary = 0, n_Temperature=0,n_Potential=0;
        int n_Node = 0, n_Element=0,nm_Node=0,nm_Boundary=0,nm_Potential=0,nm_Temperature=0;	
        int x,node,i, matprop_id=1;
	int  material_id;
        int elno,matid, dummyint[4];
        int Elem_class[2] = {0, 0};
        int class[2] ={0, 0}, classh[2]={0,0},class3a[2] ={0, 0},class3b[2] ={0, 0},class3c[2] ={0, 0},class3d[2] ={0, 0};		
        int maxiter=30,loadsteps=1;
        int bc_ux,bc_uy,bc_uz,bc_volt,mm_Boundary=0,mm_Potential=0,prev_node=0,nnn_Potential=0,nnn_Boundary=0;
	int counter=0, counter0=4;
	int mat_def=1;	
	int counter1, counter2, counter3, counter4, counter5, typecounter1=0, typecounter2=0,typecounter3=0,typecounter4=0;
	int nodcon[8];
	int file_exists,F_OK;
	int NSRLcounter=0,NKTPcounter=0,NORDRcounter=0;
	int classcounter1=0,classcounter2=0,classcounter3=0,classcounter4=0;
	int cls=0;
	int n_Load=0,n_PressL=0,n_Body=0,n_BodyL=0;
	int face;
	
	double start_time=0.000,end_time=1.000,time_step=1.0000;
	double Press;
	double eval=8.8541e-12;//farad per meter (permittivity of vacuum)
	double ux,uy,uz,stepsize,load_x=0.0,load_y=0.0,load_z=0.0,force_val;
	double Omega_z,Body_loadx,Body_loady,Body_loadz;
	double disp,disp_ux,disp_uy, disp_uz,volt,voltage, mat_val;
 	double dummy[4],var=0.000;
        double itol = 5e-5;
	double node_x,node_y,node_z;		
	double perm[5]={0.0,0.0,0.0,0.0,0.0}, elecond[5]={0.0,0.0,0.0,0.0,0.0}, themcond[5]={0.0,0.0,0.0,0.0,0.0}, heat[5]={0.0,0.0,0.0,0.0,0.0};
	double mat[] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0000.0,0.0,0.00000,0.00,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

//=======================================================================================================
file0 = fopen(argv[1], "r"); // this is NISA input (.nis) file
file1 = fopen("coordinate.dat", "w+"); // temp file for storing node and its x,y,z coordinates with deletion of ',' character from NISA file
file3a =fopen("nodconnecta.dat","w+");
file3b =fopen("nodconnectb.dat","w+");
file3c =fopen("nodconnectc.dat","w+");
file3d =fopen("nodconnectd.dat","w+");
file4=fopen("materialprop.dat","w+");// E,nu,density information
file5=fopen("event.dat","w+");
file6=fopen("boundary.dat","w+");//boundary conditions displacements only
file7=fopen("temperature.dat","w+");//temperature boundary conditions
file8=fopen("potential.dat","w+");//voltage specifications
file9=fopen("temp.dat","w+");//combined boundary conditions
fin=fopen("finpt.dat","w+");// first input file 
dens=fopen("density.dat","w+");//second input file
dens_data=fopen("density_data.dat","w+");//second input file
type1 =fopen("eltp1.dat","w+");
type2 =fopen("eltp2.dat","w+");
type3 =fopen("eltp3.dat","w+");
type4 =fopen("eltp4.dat","w+");
tp1 =fopen("one.dat","w+");
tp2=fopen("two.dat","w+");
tp3 =fopen("three.dat","w+");
tp4 =fopen("four.dat","w+");

file11 =fopen("load.dat","w+");
file12 =fopen("press.dat","w+");

//------------
filec1=fopen("CoupledEData1.txt","rb+");//coupled data 1
if(filec1==NULL)
{filec1=fopen("CoupledEData1.txt","wb");
fprintf(filec1,"%lE\n%lE\n%lE\n%lE\n", var,var,var,var);
	rewind(filec1);}
else{;}
//-----------
filec2=fopen("CoupledEData2.txt","rb+");//coupled data 2
//------------
filec2=fopen("CoupledEData2.txt","rb+");//coupled data 1
if(filec2==NULL)
{filec2=fopen("CoupledEData2.txt","wb");
fprintf(filec2,"%lE\n%lE\n%lE\n%lE\n", var,var,var,var);
	rewind(filec2);}
else{;}
//-----------

filec3=fopen("CoupledEData3.txt","rb+");//coupled data 3
//------------
filec3=fopen("CoupledEData3.txt","rb+");//coupled data 1
if(filec3==NULL)
{filec3=fopen("CoupledEData3.txt","wb");
fprintf(filec3,"%lE\n%lE\n%lE\n%lE\n", var,var,var,var);
	rewind(filec3);}
else{;}
//-----------

filec4=fopen("CoupledEData4.txt","rb+");//coupled data 4
//------------
filec4=fopen("CoupledEData4.txt","rb+");//coupled data 1
if(filec4==NULL)
{filec4=fopen("CoupledEData4.txt","wb");
fprintf(filec4,"%lE\n%lE\n%lE\n%lE\n", var,var,var,var);
	rewind(filec4);}
else{;}
//-----------

filec5=fopen("CoupledEData5.txt","rb+");//coupled data 5
//------------
filec5=fopen("CoupledEData5.txt","rb+");//coupled data 1
if(filec5==NULL)
{filec5=fopen("CoupledEData5.txt","wb");
fprintf(filec5,"%lE\n%lE\n%lE\n%lE\n", var,var,var,var);
	rewind(filec5);}
else{;}
//-----------

file10=fopen("CoupledEData","r");//coupled data

//------------
timedata=fopen("TimeData.txt","rb+");//coupled data 1
if(timedata==NULL)
{timedata=fopen("TimeData.txt","wb");
fprintf(timedata,"%lE\n%lE\n%lE", start_time,time_step,end_time);
	rewind(timedata);}
else{;}
//-----------

while((fgets(line,_length,file0))!=NULL)
   {  
          if (line[0]=='*')
                 if (!strncmp(line,string[0],6))
                      status = _TITLE;
            else if (!strncmp(line,string[1],7))
                      status = _ELTYPE;
            else if (!strncmp(line,string[2],6))
                      status = _NODES;
            else if (!strncmp(line,string[3],9))
                      status = _ELEMENTS;
            else if (!strncmp(line, string[4],9))
                      status = _MATERIAL;
            else if (!strncmp(line, string[5],6))
                      status = _EVENTS;
            else if (!strncmp(line, string[6],14))
                      status = _MAXITERATIONS;
            else if (!strncmp(line, string[7],7))
                      status = _SPDISP;
            else if (!strncmp(line, string[8],9))
                      status = _NDTEMPER;
            else if (!strncmp(line,string[9],7))
                      status = _SPFPOT;
	    else if (!strncmp(line,string[10],5))
                      status = _STEP;
 	    else if (!strncmp(line,string[11],7))
                      status = _CFORCE;
 	    else if (!strncmp(line,string[12],6))
                      status = _PRESS;
 	    else if (!strncmp(line,string[13],5))
                      status = _BODY;

            else if (line[1] == '*')
                     ;
            else 
                      status = _JUNK;         
   else
             switch(status)

                  {
                case _TITLE :
     				   x=0;
			       while(line[x])
					if(line[x]=='\r' || line[x]=='\n')
				{ linedump[x]=' ';
					x++;
				}
				else
				{ linedump[x]=line[x];
					x++;
				}
				fprintf(fin,"%s",linedump);

                case _ELTYPE : 
                               sscanf(line, "%d,%d,%d\n", &NSRL, &NKTP, &NORDR);
				NSRL_var[NSRLcounter++]=NSRL;
				NKTP_var[NKTPcounter++]=NKTP;
				NORDR_var[NORDRcounter++]=NORDR;
                               if (NORDR == 10)
					
                                  {	if(NKTP ==4)
					{strcpy(eltype_w,"b6_3");}
					else if(NKTP==31)
  					{strcpy(eltype_hyb_w,"w6_3");}
					else{;}			
				  }  
			       else
				  {     if(NKTP==4)
					{strcpy(eltype,"b8_2");}
					else if(NKTP==9)
 					{strcpy(eltype_hyb,"s8_2");}
					else{;}
			          }
				break;
                case _NODES :  
     				n_Node++;
				nm_Node++;
				x = 0;
				while (line[x])
					if (line[x++] == ',')
						line[x-1] = ' ';  
                               		fputs(line,file1);
				break;
                
               case _ELEMENTS :typecounter1++;
				n_Element++;
				  sscanf(line,"%d,%d,%d\n", &elno, &matid, &NSRL);
                                     x = 0;
					while(line[x])
						if(line[x++] == ',')
						    line[x-1] = ' '; 
								fputs(line, file3a);
			 					break;

                case _MATERIAL :
                                        
					n_Material++;
					x = 0;
					while(line[x])	
						if(line[x++] == ',')
							line[x-1] = ' ';
					fputs(line,file4);
					break;
  

                case _EVENTS :

   					fputs(line,file5);
					break;

                case _MAXITERATIONS :

					break;
                case _SPDISP :
					n_Boundary++;
					nm_Boundary++;
					nnn_Boundary++;
					x = 0;
					while(line[x])
						if(line[x++] == ',')
							line[x-1] = ' ';
					fputs(line, file6);
				
					break;
                case _NDTEMPER :
                   			n_Temperature++;
					nm_Temperature++;
					x=0;
					while(line[x])
						if(line[x++] == ',')
							line[x-1]=' ';
					fputs(line, file7);
					break;
                case _SPFPOT : 
					n_Potential++;
					nm_Potential++;	
					nnn_Potential++;
					x =0;
					while(line[x])
						if(line[x++] == ',')
							line[x-1]=' ';
						fputs(line,file8);
						
					break;
		 case _STEP : 
				
				 sscanf(line,"%lf\n", &stepsize);
					loadsteps=1.0/stepsize;
						
					break;
 		case _CFORCE :
					n_Load++;
					x = 0;
					while(line[x])
						if(line[x++] == ',')
							line[x-1] = ' ';
					fputs(line, file11);
				
					break;
		case _PRESS :
					n_PressL++;
					x = 0;
					while(line[x])
						if(line[x++] == ',')
							line[x-1] = ' ';
					fputs(line, file12);
				
					break;
		case _BODY :
					n_BodyL = 1;
					sscanf(line,"%lf, %lf, %lf, %lf, %lf, %lf\n",&dummy[0],&dummy[1],&Omega_z,&Body_loadx,&Body_loady,&Body_loadz);
					break;


                case _JUNK: 
                            break;

                    }

    }

fclose(file0);
// All data is read except material properties in Coupled data


//-------------------------------------------event 
rewind(file5);
      while(fgets(line, sizeof(line), file5) != NULL)
      {   
		   if (!strncmp(line,event_string[0],4))
  		sscanf(line,"%s=%s %d",dummy_string[0],dummy_string[1],&loadsteps);
              else if (!strncmp(line,event_string[1],4))
		sscanf(line,"%s=%d",dummy_string[0],&maxiter);
              else if (!strncmp(line,event_string[2],4))
      	        sscanf(line,"%s=%lf,%lf,%lf",dummy_string[0],&dummy[0],&dummy[1],&itol);
      }

fclose(file5);
//----------------------------------------------tolerance + no. of nodes and d.o.f + nodal coordinate data in finpt.dat
rewind(file1);
	fprintf(fin,"\n %lE \n \n %6d %d \n",itol,n_Node,3);
		while(n_Node--)
			{ fscanf(file1,"%d %lf %lf %lf %d \n",&node,&node_x,&node_y,&node_z,&dummyint[0]);
			  fprintf(fin,"\n %6d %lE %lE %lE",node,node_x*1e-6,node_y*1e-6,node_z*1e-6);
			}
fclose(file1);
//-------------------------------------------------------------------------------------------print no. of d.o.f. per node in finpt.dat

fprintf(fin,"\n\n %6d\n",4);

//----------------------------------------------------  writing file 9 which combines all boundary conditions
rewind(file6);
while(nnn_Boundary--)
{fscanf(file6,"%d %s %lf %d\n", &node,disp_type,&disp,&dummyint[0]);
fprintf(file9,"%6d %s %lf\n", node,disp_type,disp);
}
fclose(file6);

rewind(file8);
while(nnn_Potential--)
{fscanf(file8,"%d %lf\n",&node,&voltage);
fprintf(file9,"%6d %s %lE\n ", node, &Pot[0], voltage);
}
//fclose(file8);

//------------------------------------------------------------------------boundary conditions writing in finpt
rewind(file9);
//i=2;
for(i=0;i<=nm_Node;i++)
//while(nm_Node--)
  {	bc_ux=0;
	bc_uy=0;
	bc_uz=0;
	bc_volt=0;
	disp_ux=0.00;
	disp_uy=0.00;
	disp_uz=0.00;
	volt=100000.00;
	prev_node=0;
	counter++;
  fscanf(file9,"%d %s %lf \n",&node,disp_type,&disp);
	mm_Boundary=nm_Boundary+nm_Potential;
	while(mm_Boundary--)
	{	if(node == counter)
			
		{	if(disp_type[1] == 'X')
				{	bc_ux=1;
					disp_ux=disp;
					prev_node=node;
				}
			else if(disp_type[1] == 'Y')
				{	bc_uy=1;
					disp_uy=disp;
					prev_node=node;
				}
			else if(disp_type[1] == 'Z')
				{	bc_uz=1;	
					disp_uz=disp;
					prev_node=node;
				}
			else if(disp_type[0] == 'V')
				{	bc_volt=1;
					volt=disp;
					prev_node=node;	
				}
			else if (disp_type[0]=='R')
				;
		}	
		else	{ ;
			//fscanf(file9,"%d %s %lf \n",&node,disp_type,&disp);
			}


	 fscanf(file9,"%d %s %lf \n",&node,disp_type,&disp);
	}
if(prev_node==counter)
{fprintf(fin,"%6d %6d %6d %6d %6d %6d %6d\n", prev_node,prev_node,1,bc_ux,bc_uy,bc_uz,bc_volt);
if(volt !=100000.00)
{fprintf(dens_data,"%6d %6d %6d %lf %lf %lf %lf\n", prev_node,prev_node,1,disp_ux,disp_uy,disp_uz,volt);		
}
else{;}
}
else{;}
rewind(file9);
 } 

fclose(file9);
//---------------------------- zero end line after boundary conditions data
fprintf(fin,"%6d %6d %6d %6d %6d %6d %6d\n", 0, 0, 0, 0, 0, 0, 0);
//----------------------------------------------------------------------------------------------




//---------------------------- scaning material properties


rewind(filec1);
fscanf(filec1,"%lf\n",&perm[0]);
fscanf(filec1,"%lf\n",&heat[0]);
fscanf(filec1,"%lf\n",&elecond[0]);
fscanf(filec1,"%lf",&themcond[0]);
fclose(filec1);

rewind(filec2);
fscanf(filec2,"%lf\n",&perm[1]);
fscanf(filec2,"%lf\n",&heat[1]);
fscanf(filec2,"%lf\n",&elecond[1]);
fscanf(filec2,"%lf",&themcond[1]);
fclose(filec2);

rewind(filec3);
fscanf(filec3,"%lf\n",&perm[2]);
fscanf(filec3,"%lf\n",&heat[2]);
fscanf(filec3,"%lf\n",&elecond[2]);
fscanf(filec3,"%lf",&themcond[2]);
fclose(filec3);

rewind(filec4);
fscanf(filec4,"%lf\n",&perm[3]);
fscanf(filec4,"%lf\n",&heat[3]);
fscanf(filec4,"%lf\n",&elecond[3]);
fscanf(filec4,"%lf",&themcond[3]);
fclose(filec4);

rewind(filec5);
fscanf(filec5,"%lf\n",&perm[4]);
fscanf(filec5,"%lf\n",&heat[4]);
fscanf(filec5,"%lf\n",&elecond[4]);
fscanf(filec5,"%lf",&themcond[4]);
fclose(filec5);


//=========================================================================================================================== write material property data

 rewind(file4);
	fprintf(fin,"\n %6d %6d",matprop_id,30);
	fscanf(file4, "%s %d %d %lf \n",mat_prop,&material_id,&dummyint[0],&mat_val);
	while(n_Material--)
	{
		if(material_id != mat_def)
		{       mat[10]=perm[mat_def-1]*eval;
			mat[11]=elecond[mat_def-1];
			fprintf(fin,"\n %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE",mat[0],mat[1],mat[2],mat[3],mat[4],mat[5],mat[6],mat[7],mat[8],mat[9],mat[10],mat[11],mat[12],mat[13],mat[14],mat[15],mat[16],mat[17],mat[18],mat[19],mat[20],mat[21],mat[22],mat[23],mat[24],mat[25],mat[26],mat[27],mat[28],mat[29]);
			mat_def = material_id;
			fprintf(fin,"\n %6d %6d",material_id,30);
			mat[0] = mat[1] = mat[8] =mat[10]=mat[11]= 0.0;
			
				if (mat_prop[0] == 'E')
				mat[0] = mat_val;
			else if (mat_prop[0] == 'N')
				mat[1] = mat_val;
			else if (mat_prop[0] == 'D')
				mat[8] = mat_val;
            
				}

		else
			if (mat_prop[0] == 'E')
				mat[0] = mat_val;
			else if (mat_prop[0] == 'N')
				mat[1] = mat_val;
			else if (mat_prop[0] == 'D')
				mat[8] = mat_val;
                		fscanf(file4, "%s %d %d %lf \n",mat_prop,&material_id,&dummyint[0],&mat_val);
	}
                        mat[10]=perm[mat_def-1]*eval;
			mat[11]=elecond[mat_def-1];

	fprintf(fin,"\n %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE %lE",mat[0],mat[1],mat[2],mat[3],mat[4],mat[5],mat[6],mat[7],mat[8],mat[9],mat[10],mat[11],mat[12],mat[13],mat[14],mat[15],mat[16],mat[17],mat[18],mat[19],mat[20],mat[21],mat[22],mat[23],mat[24],mat[25],mat[26],mat[27],mat[28],mat[29]);
	fprintf(fin,"\n %6d %6d %6d %6d %6d %6d %6d",0,0,0,0,0,0,0);
	fclose(file4);
//========================================================================================================================================================
cls=0.5*typecounter1;
fprintf(fin,"\n\n % 6d",cls);
rewind(file3a);
while((fgets(line,_length,file3a))!=NULL)
{typecounter2++;
 if(typecounter2%2!=0) 
{ sscanf(line,"%d %d %d %d %d\n", &elno, &matid, &NSRL, &dummyint[0],&dummyint[1]);
 }
else{
if(NSRL==1)
{ if(NKTP_var[0]==4)
	  { if(NORDR_var[0]==1)
		{ 	fprintf(fin,"\n'%s'",eltype);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d",1,8,8,6);
			sscanf(line,"%d %d %d %d %d %d %d %d\n",&nodcon[0],&nodcon[1],&nodcon[2],&nodcon[3],&nodcon[4],&nodcon[5],&nodcon[6],&nodcon[7]);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d %6d %6d",elno,nodcon[0],nodcon[1],nodcon[2],nodcon[3],nodcon[4],nodcon[5],nodcon[6],nodcon[7],matid,1);
		}
	  else if(NORDR_var[0]==10)
		{	fprintf(fin,"\n'%s'",eltype_w);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d",1,6,6,6);
			sscanf(line,"%d %d %d %d %d %d \n",&nodcon[0],&nodcon[1],&nodcon[2],&nodcon[3],&nodcon[4],&nodcon[5]);
					fprintf(fin,"\n % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d",elno,nodcon[0],nodcon[1],nodcon[2],nodcon[3],nodcon[4],nodcon[5],matid,1);
		}
	else{;}	
	  }
  else if(NKTP_var[0]==9)
 
		{ 	fprintf(fin,"\n'%s'",eltype_hyb);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d",1,8,8,6);
			sscanf(line,"%d %d %d %d %d %d %d %d \n",&nodcon[0],&nodcon[1],&nodcon[2],&nodcon[3],&nodcon[4],&nodcon[5],&nodcon[6],&nodcon[7]);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d %6d %6d",elno,nodcon[0],nodcon[1],nodcon[2],nodcon[3],nodcon[4],nodcon[5],nodcon[6],nodcon[7],matid,1);
		}
 else if(NKTP_var[0]==31)
		{ 	fprintf(fin,"\n'%s'",eltype_hyb_w);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d",1,6,6,6);
			sscanf(line,"%d %d %d %d %d %d \n",&nodcon[0],&nodcon[1],&nodcon[2],&nodcon[3],&nodcon[4],&nodcon[5]);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d",elno,nodcon[0],nodcon[1],nodcon[2],nodcon[3],nodcon[4],nodcon[5],matid,1);
		}
else {;}
}



else if(NSRL==2)
{ if(NKTP_var[1]==4)
	  { if(NORDR_var[1]==1)
		{ 	fprintf(fin,"\n'%s'",eltype);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d",1,8,8,6);
			sscanf(line,"%d %d %d %d %d %d %d %d \n",&nodcon[0],&nodcon[1],&nodcon[2],&nodcon[3],&nodcon[4],&nodcon[5],&nodcon[6],&nodcon[7]);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d %6d %6d",elno,nodcon[0],nodcon[1],nodcon[2],nodcon[3],nodcon[4],nodcon[5],nodcon[6],nodcon[7],matid,1);
		}
	  else if(NORDR_var[1]==10)
		{	fprintf(fin,"\n'%s'",eltype_w);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d",1,6,6,6);
			sscanf(line,"%d %d %d %d %d %d\n",&nodcon[0],&nodcon[1],&nodcon[2],&nodcon[3],&nodcon[4],&nodcon[5]);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d",elno,nodcon[0],nodcon[1],nodcon[2],nodcon[3],nodcon[4],nodcon[5],matid,1);
		}
	else{;}	

	  }
  else if(NKTP_var[1]==9)
 
		{ 	fprintf(fin,"\n'%s'",eltype_hyb);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d",1,8,8,6);
			sscanf(line,"%d %d %d %d %d %d %d %d \n",&nodcon[0],&nodcon[1],&nodcon[2],&nodcon[3],&nodcon[4],&nodcon[5],&nodcon[6],&nodcon[7]);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d %6d %6d",elno,nodcon[0],nodcon[1],nodcon[2],nodcon[3],nodcon[4],nodcon[5],nodcon[6],nodcon[7],matid,1);
		}
 else if(NKTP_var[1]==31)
 
		{ 	fprintf(fin,"\n'%s'",eltype_hyb_w);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d",1,6,6,6);
			sscanf(line,"%d %d %d %d %d %d\n",&nodcon[0],&nodcon[1],&nodcon[2],&nodcon[3],&nodcon[4],&nodcon[5]);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d",elno,nodcon[0],nodcon[1],nodcon[2],nodcon[3],nodcon[4],nodcon[5],matid,1);
		}
else {;}
}
if(NSRL==3)
{ if(NKTP_var[2]==4)
	  { if(NORDR_var[2]==1)
		{ 	fprintf(fin,"\n'%s'",eltype);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d",1,8,8,6);
			sscanf(line,"%d %d %d %d %d %d %d %d  \n",&nodcon[0],&nodcon[1],&nodcon[2],&nodcon[3],&nodcon[4],&nodcon[5],&nodcon[6],&nodcon[7]);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d %6d %6d",elno,nodcon[0],nodcon[1],nodcon[2],nodcon[3],nodcon[4],nodcon[5],nodcon[6],nodcon[7],matid,1);
		}
	  else if(NORDR_var[2]==10)
		{	fprintf(fin,"\n'%s'",eltype_w);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d",1,6,6,6);
			sscanf(line,"%d %d %d %d %d %d\n",&nodcon[0],&nodcon[1],&nodcon[2],&nodcon[3],&nodcon[4],&nodcon[5]);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d",elno,nodcon[0],nodcon[1],nodcon[2],nodcon[3],nodcon[4],nodcon[5],matid,1);
		}
	else{;}	

	  }
  else if(NKTP_var[2]==9)
 
		{ 	fprintf(fin,"\n'%s'",eltype_hyb);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d",1,8,8,6);
			sscanf(line,"%d %d %d %d %d %d %d %d \n",&nodcon[0],&nodcon[1],&nodcon[2],&nodcon[3],&nodcon[4],&nodcon[5],&nodcon[6],&nodcon[7]);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d %6d %6d",elno,nodcon[0],nodcon[1],nodcon[2],nodcon[3],nodcon[4],nodcon[5],nodcon[6],nodcon[7],matid,1);
		}
 else if(NKTP_var[2]==31)
 
		{ 	fprintf(fin,"\n'%s'",eltype_hyb_w);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d",1,6,6,6);
			sscanf(line,"%d %d %d %d %d %d \n",&nodcon[0],&nodcon[1],&nodcon[2],&nodcon[3],&nodcon[4],&nodcon[5]);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d",elno,nodcon[0],nodcon[1],nodcon[2],nodcon[3],nodcon[4],nodcon[5],matid,1);
		}
else {;}
}
if(NSRL==4)
{ if(NKTP_var[3]==4)
	  { if(NORDR_var[3]==1)
		{ 	fprintf(fin,"\n'%s'",eltype);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d",1,8,8,6);
			sscanf(line,"%d %d %d %d %d %d %d %d \n",&nodcon[0],&nodcon[1],&nodcon[2],&nodcon[3],&nodcon[4],&nodcon[5],&nodcon[6],&nodcon[7]);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d %6d %6d",elno,nodcon[0],nodcon[1],nodcon[2],nodcon[3],nodcon[4],nodcon[5],nodcon[6],nodcon[7],matid,1);
		}
	  else if(NORDR_var[3]==10)
		{	fprintf(fin,"\n'%s'",eltype_w);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d",1,6,6,6);
			sscanf(line,"%d %d %d %d %d %d \n",&nodcon[0],&nodcon[1],&nodcon[2],&nodcon[3],&nodcon[4],&nodcon[5]);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d",elno,nodcon[0],nodcon[1],nodcon[2],nodcon[3],nodcon[4],nodcon[5],matid,1);
		}
	else{;}	

	  }
  else if(NKTP_var[3]==9)
 
		{ 	fprintf(fin,"\n'%s'",eltype_hyb);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d",1,8,8,6);
			sscanf(line,"%d %d %d %d %d %d %d %d \n",&nodcon[0],&nodcon[1],&nodcon[2],&nodcon[3],&nodcon[4],&nodcon[5],&nodcon[6],&nodcon[7]);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d %6d %6d",elno,nodcon[0],nodcon[1],nodcon[2],nodcon[3],nodcon[4],nodcon[5],nodcon[6],nodcon[7],matid,1);
		}
 else if(NKTP_var[3]==31)
 
		{ 	fprintf(fin,"\n'%s'",eltype_hyb_w);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d",1,6,6,6);
			sscanf(line,"%d %d %d %d %d %d \n",&nodcon[0],&nodcon[1],&nodcon[2],&nodcon[3],&nodcon[4],&nodcon[5]);
			fprintf(fin,"\n % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d % 6d",elno,nodcon[0],nodcon[1],nodcon[2],nodcon[3],nodcon[4],nodcon[5],matid,1);
		}
else {;}
}
else{;}





}
}
fclose(file3a);
//fprintf(fin,"\n %6d %6d %6d %6d %6d",0,0,0,0,0);
//================================================================





//========================================================================================================================

    rewind(file11);
	fprintf(fin,"\n ");
	if (n_Load !=0)
	{
		fscanf(file11, "%d %s %lf %d %d \n",&node,load_def,&force_val,&dummyint[0],&dummyint[1]);
		prev_node = node;
		while(n_Load--)
		{
			if(node != prev_node)
			{
				fprintf(fin,"\n % 6d %lE %lE %lE %lE",prev_node,load_x,load_y,load_z,0.0);
				prev_node = node;
				load_x = load_y = load_z = 0.0;
				if(load_def[1] == 'X')
					load_x = force_val;
				else if(load_def[1] == 'Y')
					load_y = force_val;
				else
					load_z = force_val;
			}
			else
				if(load_def[1] == 'X')
					load_x = force_val;
				else if(load_def[1] == 'Y')
					load_y = force_val;
				else
					load_z = force_val;

			fscanf(file11, "%d %s %lf %d %d \n",&node,load_def,&force_val,&dummyint[0],&dummyint[1]);

		}
		fprintf(fin,"\n % 6d %lE %lE %lE %lE",node,load_x,load_y,load_z,0.0);
                fprintf(fin,"\n %6d %6d %6d %6d %6d",0,0,0,0,0);

	}
	else
	fprintf(fin,"\n %6d %6d %6d %6d %6d",0,0,0,0,0);
		fclose(file11);
fprintf(fin,"\n ");
//=========================================================================================================================
fclose(fin);



rewind(timedata);
 fscanf(timedata,"%lf\n",&start_time );
 fscanf(timedata,"%lf\n",&end_time );
 fscanf(timedata,"%lf\n",&time_step );
fclose(timedata);




//=========================================================================================================================

fprintf(dens,"% 6d  % 6d % lE % lE % lE % lE %6d ", 0,1,end_time,time_step,1.0000,0.0,loadsteps);

//=========================================================================================================================


//==========================================================================================================================
       rewind(file12);
	if (n_BodyL != 0 && n_PressL != 0)
	{
		fprintf(dens,"% 6d % 6d \n",1,1);
		fscanf(file12,"%d %d %d %d %lf %d %d \n",&elno,&face,&dummyint[0],&dummyint[1],&Press,&dummyint[2],&dummyint[3]);
		if (dummyint[0]>0)
		{
			fgets(line, sizeof(line), file12);
			sscanf(line,"%lf \n",&Press);
			dummyint[0] -= 1;
		}
		fprintf(dens,"% lE % lE % lE % lE",Press,0.0,0.0,0.0);
		while(n_PressL)
		{
			fprintf(dens,"\n % 6d % 6d % 6d % 6d",elno,elno,1,face);
			fscanf(file12,"%d %d %d %d %lf %d %d \n",&elno,&face,&dummyint[0],&dummyint[1],&Press,&dummyint[2],&dummyint[3]);
			if (dummyint[0]>0)
			{
				fgets(line, sizeof(line), file12);
				dummyint[0] -= 1;
				n_PressL -= 1;
			}
			n_PressL -= 1;
		}
		fprintf(dens,"\n % 6d % 6d % 6d % 6d",0,0,0,0);
		fprintf(dens,"\n % lE % lE  % lE %lE",Body_loadx,Body_loady,Body_loadz,Omega_z);
		fprintf(dens,"\n% 6d % 6d % 6d % 6d % 6d % 6d % 6d \n",0,0,0,0,0,0,0);
	}
	else if (n_BodyL == 0 && n_PressL !=0)
	{
		fprintf(dens,"% 6d % 6d \n",1,0);
		fscanf(file12,"%d %d %d %d %lf %d %d \n",&elno,&face,&dummyint[0],&dummyint[1],&Press,&dummyint[2],&dummyint[3]);
		if (dummyint[0]>0)
		{
			fgets(line, sizeof(line), file12);
			sscanf(line,"%lf \n",&Press);
			dummyint[0] -= 1;
		}
		fprintf(dens,"% lE % lE % lE % lE",Press,0.0,0.0,0.0);
		while(n_PressL)
		{
			fprintf(dens,"\n % 6d % 6d % 6d % 6d",elno,elno,1,face);
			fscanf(file12,"%d %d %d %d %lf %d %d \n",&elno,&face,&dummyint[0],&dummyint[1],&Press,&dummyint[2],&dummyint[3]);
			if (dummyint[0]>0)
			{
				fgets(line, sizeof(line), file12);
				dummyint[0] -= 1;
				n_PressL -= 1;
			}
			n_PressL -= 1;
		}
		fprintf(dens,"\n % 6d % 6d % 6d % 6d\n",0,0,0,0);
	//	fprintf(dens,"\n % 6d % 6d % 6d % 6d % 6d % 6d % 6d \n",0,0,0,0,0,0,0);
	}
	else if (n_BodyL !=0 && n_PressL == 0)
	{
		fprintf(dens,"% 6d % 6d \n",0,1);
		fprintf(dens,"% lE, % lE, % lE, %lE\n",Body_loadx,Body_loady,Body_loadz,Omega_z);
		fprintf(dens,"\n % 6d, % 6d % 6d % 6d % 6d % 6d % 6d \n",0,0,0,0,0,0,0);
	}
	else
	{
		fprintf(dens,"% 6d % 6d \n",0,0);
		//fprintf(dens,"\n % 6d % 6d % 6d % 6d % 6d % 6d % 6d \n",0,0,0,0,0,0,0);
	}

	fclose(file12);
//---------------------------------------specified nodal potentials only (ground+nonzero)-------------------------------------------
//rewind(file8);
//ux=uy=uz=0.0000;
// while((fgets(line,_length,file8))!=NULL)
//{	  sscanf(line, "%d  %lf \n",&node,&volt);
//          fprintf(dens,"\n %6d %6d %6d %lf %lf %lf %lf",node,node,1,ux,uy,uz,volt);
//}
//	fprintf(dens,"\n %6d %6d %6d %6d %6d %6d %6d",0,0,0,0,0,0,0);
//fclose(file8);	
//========================
//---------------------------------------specified nodal potentials (ground+nonzero)+and specified displacements on nodes at which voltage also prescribed-------------------------------------------
rewind(dens_data);
ux=uy=uz=0.0000;
 while((fgets(line,_length,dens_data))!=NULL)
{	   sscanf(line," %d %d %d %lf %lf %lf %lf\n",&node,&node,&dummyint[0],&ux,&uy,&uz,&volt);
           fprintf(dens," %6d %6d %6d %lf %lf %lf %lf\n",node,node,1,ux,uy,uz,volt);
}
	fprintf(dens," %6d %6d %6d %6d %6d %6d %6d",0,0,0,0,0,0,0);
fprintf(dens,"\n ");
fclose(dens);
//========================



//========================
fclose(file0);
fclose(type1);
fclose(type2);
fclose(type3);
fclose(type4);
fclose(tp1);
fclose(tp2);
fclose(tp3);
fclose(tp4);

fclose(file3b);
fclose(file3c);
fclose(file3d);
fclose(file7);
fclose(file8);
fclose(dens_data);
remove("coordinate.dat");
remove("nodconnecta.dat");
remove("nodconnectb.dat");
remove("nodconnectc.dat");
remove("nodconnectd.dat");
remove("materialprop.dat");
remove("event.dat");
remove("boundary.dat");
remove("temperature.dat");
remove("potential.dat");
remove("temp.dat");
remove("density_data.dat");
remove("eltp1.dat");
remove("eltp2.dat");
remove("eltp3.dat");
remove("eltp4.dat");
remove("one.dat");
remove("two.dat");
remove("three.dat");
remove("four.dat");
remove("load.dat");
remove("press.dat");
remove("CoupledData");
remove("CoupledEData1.txt");
remove("CoupledEData2.txt");
remove("CoupledEData3.txt");
remove("CoupledEData4.txt");
remove("CoupledEData5.txt");
remove("TimeData.txt");



}
