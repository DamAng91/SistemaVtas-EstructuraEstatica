#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

using namespace std;

const int MAX_REGS = 800;
const int MAX_VENS = 100;

typedef char str20[20];
typedef unsigned short ushort;

void StrToStr (str20 cad1, str20 cad2) {
	
	for (int i=0; i<20; i++){
		cad2[i] = cad1[i];
	}
}

void UshortToUshort (ushort num1, ushort &num2){
	
	num2 = num1;
}

void FloatToFloat (float num1, float &num2) {
	
	num2 = num1;
}

struct sFec {
	ushort dd,	
	mm,
	aaaa;
}; 

struct sVen { //struct VentasAF
	ushort codVen,
	cant;
	str20 descr; //codArt = descr
	float preUni;
	sFec FechaVta; //Fecha de venta
}; //Estructura de ventas

struct sTotVen { // struct total vendedor
	ushort	codVen,
	canTot;
	float impTot;
}; //Estructura Total de Ventas x Vendedor

bool LeeUnaVta (ifstream &VentasAF, sVen &rVendedor) {//&vAF = VentasAF; sVen &rVen = rVendedor

	VentasAF >> rVendedor.codVen;
	if(rVendedor.codVen) {
		VentasAF >> rVendedor.cant;
		VentasAF.ignore();
		VentasAF.get(rVendedor.descr,20);
		VentasAF >> rVendedor.preUni;
		VentasAF >> rVendedor.FechaVta.dd;
		VentasAF >> rVendedor.FechaVta.mm;
		VentasAF >> rVendedor.FechaVta.aaaa;
	
		return true;
	}

	return false;
}

void ProcVentas(ifstream &VtasAF, sVen vrVtas[],ushort &cVtas) {//archivo, vector, variable cantidad ventas
	while(!VtasAF.eof()) {
		//lee una venta y la almacena en el registro ventas
		//si leyo, entonces es verdadero entonces carga en el arreglo e incremento un contador de ventas llamado cVentas
	
		sVen rVen;
		
		if(LeeUnaVta(VtasAF, rVen)) {
			if(cVtas < MAX_REGS) {

				UshortToUshort(rVen.codVen, vrVtas[cVtas].codVen);
				UshortToUshort(rVen.cant, vrVtas[cVtas].cant);
				StrToStr(rVen.descr, vrVtas[cVtas].descr);
				FloatToFloat(rVen.preUni, vrVtas[cVtas].preUni);
				UshortToUshort(rVen.FechaVta.dd, vrVtas[cVtas].FechaVta.dd);
				UshortToUshort(rVen.FechaVta.mm, vrVtas[cVtas].FechaVta.mm);
				UshortToUshort(rVen.FechaVta.aaaa, vrVtas[cVtas].FechaVta.aaaa);

				cVtas++;
			}
		}
	}
}

void IntCmb(sVen &rVtas1,sVen &rVtas2) {
	
	sVen aux = rVtas1;
	rVtas1 = rVtas2;
	rVtas2 = aux;	
} //Intercambio de pos actual y pos siguiente

void OrdxBur(sVen vrVtas[],ushort card) {
	
	bool ordenado;
	ushort li=0;
	
	do {
		li++;
		ordenado = true;
			
		int i;
		for(i=0; i<card-li;i++)
			if(vrVtas[i].codVen>vrVtas[i+1].codVen){
				IntCmb (vrVtas[i], vrVtas[i+1]);
				ordenado=false;
			}		
	} 
	while(!ordenado);
	
} //Ordenamiento x burbujeo vrVtas.codVen 

void IntCmb(sTotVen &rTot1, sTotVen &rTot2) {
	
	sTotVen auxTot = rTot1;
	rTot1 = rTot2;
	rTot2 = auxTot;
}

void OrdxBur(sTotVen vrTot[], ushort card){
	bool ordenado;
	ushort li=0;
	
	do {
		li++;
		ordenado = true;
			
		int i;
		for(i=0; i<card-li;i++)
			if(vrTot[i].canTot < vrTot[i+1].canTot){
				IntCmb (vrTot[i], vrTot[i+1]);
				ordenado=false;
			}		
	}
	while(!ordenado);
} //OrdxBur cantidad total

void OrdxBur(ushort card, sTotVen vrTot[]){
	bool ordenado;
	ushort li=0;
	
	do {
		li++;
		ordenado = true;
			
		int i;
		for(i=0; i<card-li;i++)
			if(vrTot[i].impTot<vrTot[i+1].impTot){
				IntCmb (vrTot[i], vrTot[i+1]);
				ordenado=false;
			}		
	}
	while(!ordenado);
} //OrdxBur importe total

ushort ObtLong (sTotVen vrTot[]) {
	
	ushort i=0;
	
	while(i<MAX_VENS && vrTot[i].codVen!=0) {
		i++;
		
	}
		return i;
} //ObtLong vrTot

ushort ObtLong (sVen vrVtas[]) {
	
	ushort i=0;
	
	while(i<MAX_REGS && vrVtas[i].codVen!=0) {
		i++;
		
	}
		return i;
} //ObtLong vrVtas

void ListadoVtasAgrupVen(ofstream &Sld, sVen vrVtas[], sTotVen vrTot[], ushort cVtas, ushort cVen) {

	ushort n=0,
	m=0,
	sumCant=0,
	cVtaMayor;
	int item=1; 
	float sum=0,
	sumTot=0,
	importeMayor=0;
	
	n= ObtLong (vrVtas);
	OrdxBur(vrVtas,n);
		
	for (int i=0; i<n; i++) {
		
		if (vrVtas[i].codVen) {
			
			if (i>0 && vrVtas[i].codVen == vrVtas[i-1].codVen) { 
	
				item ++;
				Sld << setw(4) << item;
				Sld << setw(5) << vrVtas[i].FechaVta.dd<<"-"<< vrVtas[i].FechaVta.mm<<"-"<< vrVtas[i].FechaVta.aaaa;
				Sld << setw(6) << vrVtas[i].cant;
				Sld << setw(23) << vrVtas[i].descr; 
				Sld << setw(5) <<"$" << vrVtas[i].preUni;			

				sumCant += vrVtas[i].cant;
	      sum += vrVtas[i].cant*vrVtas[i].preUni;
	            
				if (vrVtas[i].codVen != vrVtas[i+1].codVen) {

					Sld << setw(6) <<"$"<< vrVtas[i].cant*vrVtas[i].preUni;					
					Sld << setw(6) <<"$"<< sum <<endl;
					sumTot +=sum;
					
					if (vrVtas[i].codVen && m < MAX_VENS) { //m no puede superar el valor de n:tamaño del vrVtas
						vrTot[m].codVen = vrVtas[i].codVen;
						vrTot[m].canTot = sumCant;
						vrTot[m].impTot = sum;
						m++;
					}
					
					if (importeMayor < sum) {
						importeMayor = sum;
						cVtaMayor = vrVtas[i].codVen;
					}
										
				} else 
					Sld << setw(6) <<"$"<< vrVtas[i].cant*vrVtas[i].preUni<<endl;				
	            
			} else {
				
				sum=0;
				item=1;
				sumCant=0;
	
				Sld <<"-------------------------------------------------------------------------------------"<<endl<<endl; 
				Sld <<"Cód. Vendedor: "<<vrVtas[i].codVen<<endl<<endl; 
				Sld <<"#Item     Fecha    Cant.      Descripción        Pre.Unit.  Tot.Item    Tot. Ven."<<endl;		

				Sld << setw(4) << item;
				Sld << setw(5) << vrVtas[i].FechaVta.dd<<"-"<< vrVtas[i].FechaVta.mm<<"-"<< vrVtas[i].FechaVta.aaaa;
				Sld << setw(6) << vrVtas[i].cant;
				Sld << setw(23) << vrVtas[i].descr; 
				Sld << setw(5) <<"$" << vrVtas[i].preUni;	
				
				sumCant += vrVtas[i].cant;
	      sum += vrVtas[i].cant*vrVtas[i].preUni;
	            
	      if (vrVtas[i].codVen != vrVtas[i+1].codVen) {

		    	Sld << setw(6) <<"$"<< vrVtas[i].cant*vrVtas[i].preUni;					
					Sld << setw(6) <<"$"<< sum<<endl;
					sumTot +=sum;
					
					if (vrVtas[i].codVen && m < MAX_VENS) {
						vrTot[m].codVen = vrVtas[i].codVen;
						vrTot[m].canTot = sumCant;
						vrTot[m].impTot = sum;
						m++;
					}
					
					if (importeMayor < sum) {
						importeMayor = sum;
						cVtaMayor = vrVtas[i].codVen;
					}

				} else {
		
					Sld << setw(6) <<"$"<< vrVtas[i].cant*vrVtas[i].preUni<<endl;
		
				} 
			}
		}
	} 
	
	Sld <<endl<<endl;
	Sld <<"Total General: " <<sumTot<<endl; //CodVenMayorImpt		
	Sld <<"Cód. Ven. mayor venta de importe: " << cVtaMayor<<endl; //codVenMayor	
	
	Sld <<"-------------------------------------------------------------------------------------"<<endl<<endl; 
} 

void ListadoCantTot(ofstream &Sld,sTotVen vrTot[],ushort cVen) { 
	
//	cVen = sizeof(vrTot);
		
	OrdxBur(vrTot, cVen);
	Sld<<"          Cód. Ven.          Cant.Tot."<<endl;

	for(int i=0; i<cVen; i++) {
		Sld << setw(16) <<vrTot[i].codVen << setw(19) <<vrTot[i].canTot<<endl;
	}
	
	Sld <<"-------------------------------------------------------------------------------------"<<endl<<endl; 
} //Emite Listado cantidad total ord. decrecientemente

void ListadoImpTot(ofstream &Sld,sTotVen vrTot[],ushort cVen) { 

//	cVen = sizeof(vrTot);
	
	OrdxBur(cVen, vrTot);
	Sld <<"          Cód. Ven.          Importe Total"<<endl;

	for(int i=0; i<cVen; i++) {
		Sld << setw(16) <<vrTot[i].codVen << setw(15) <<"$"<<vrTot[i].impTot<<endl;
	}
	
	Sld <<"-------------------------------------------------------------------------------------"<<endl<<endl; 
} //Emite Listado importe total x cod vendedor ord. decrecientemente


int main(){
//	ushort cantVtas = 0;
//	ushort cantVen = 0;
	
	sVen vrVendedores [MAX_REGS];
	sTotVen vrTotales [MAX_VENS];

	ifstream VentasAF("VentasFerreteria.Txt");
	ofstream SalidaAF("ListadoAF.Txt");
	
	ProcVentas(VentasAF, vrVendedores, cantVtas);
		
	ListadoVtasAgrupVen(SalidaAF, vrVendedores, vrTotales, cantVtas, canVen); 

//	cantVen = ObtLong (vrTotales);
		
	ListadoCantTot(SalidaAF, vrTotales, cantVen); 
	ListadoImpTot(SalidaAF, vrTotales, cantVen); 

	VentasAF.close();
	SalidaAF.close();
	
	return 0;
}
