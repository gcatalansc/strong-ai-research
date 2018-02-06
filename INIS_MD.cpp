//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
/*
Nombre Clase : 	  INIS_MD (Estructura de información de entrada tipo 'MD').
Fecha	     :	  11:50 pm 08-02-2017.
Dise�ador    :	  Gustavo Catalán Ulloa.
descripción	 :	  XXXXXXXXXXXX.
*/
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//librerias.
#include "INIS_MD.h"

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//espacio de nombres.
using namespace UPC;

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//#1: constructor.
INIS_MD::INIS_MD()
{
	//---------------------------------------------------------------------------------
	//inicialización de registros.
	this->EV_RG		= -1;																	//registro de evaluación.															

	//---------------------------------------------------------------------------------
	//inicialización de sub-registros.	
	this->RS_SRG	= -1;																	//estado de realidad.
	this->CS_SRG	= -1;																	//estado de congruencia.
	this->AL_SRG	= -1;																	//nivel de actividad.
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//#2: destructor.
INIS_MD::~INIS_MD()
{
	//---------------------------------------------------------------------------------
	//limpieza de buffer.
	this->Clean();
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//método para inicializar estructura.
vo INIS_MD::Initialize(in _x_length, in _y_length)
{
	//---------------------------------------------------------------------------------
	//inicialización de vector 'X'.
	this->IN_RG = new dou *[_x_length];

	//---------------------------------------------------------------------------------
	//seteo de las dimensiones del registro.
	this->X_LENGTH	= _x_length;
	this->Y_LENGTH	= _y_length;

	//---------------------------------------------------------------------------------
	//inicialización de vectores 'X'.
	for (register in i1 = 0; i1 < _x_length; i1++)
	{
		//-----------------------------------------------------------------------------
		//inicialización de vectores 'Y'.
		this->IN_RG[i1]	=	new dou[_y_length];

		//-----------------------------------------------------------------------------
		//paralelización del bucle for.
		#pragma omp parallel for

		//-----------------------------------------------------------------------------
		//bucle para llenado de la matriz.
		for (register in i2 = 0; i2 < _y_length; i2++)
		{
			//-------------------------------------------------------------------------
			//llenado de la matriz.
			this->IN_RG[i1][i2]		= -1;
		}
	}
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//#1: método para crear estructura.
vo INIS_MD::Create(dou **_input, dou _input_evaluation, in _rs_srg, dou _al_srg)
{
	//---------------------------------------------------------------------------------
	//paralelización del bucle for.
	#pragma omp parallel for

	//---------------------------------------------------------------------------------
	//seteo del registro de entrada en 'X'.
	for (in i1 = 0; i1 < this->X_LENGTH; i1++)
	{
		//-----------------------------------------------------------------------------
		//paralelización del bucle for.
		#pragma omp parallel for

		//-----------------------------------------------------------------------------
		//seteo del registro de entrada en 'Y'.
		for (in i2 = 0; i2 < this->Y_LENGTH; i2++)
		{
			//-------------------------------------------------------------------------
			//seteo del registro de entrada.
			this->IN_RG[i1][i2] = _input[i1][i2];
		}
	}

	//---------------------------------------------------------------------------------
	//seteo de registros.
	this->EV_RG		= _input_evaluation;													//registro de evaluación.

	//---------------------------------------------------------------------------------
	//seteo de sub-registros.			
	this->RS_SRG	= _rs_srg;																//estado de realidad.
	this->CS_SRG	= OFF;																	//estado de congruencia.
	this->AL_SRG	= _al_srg;																//nivel de actividad.
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//#1: método para crear una relación a la estructura.
vo INIS_MD::CreateRelation(in _rl_type, in _rl_id, in _upci_type, in _upci_id)
{
	//--------------------------------------------------------------------------------
	//seteo de uana relación a la estructura.
	this->RL.push_back(deque<in>(0));
	this->RL.back().push_back(_rl_type);
	this->RL.back().push_back(_rl_id);
	this->RL.back().push_back(_upci_type);
	this->RL.back().push_back(_upci_id);
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//#2: método para eliminar una relación a la estructura.
vo INIS_MD::DeleteRelation(in _index)
{
	//---------------------------------------------------------------------------------
	//eliminación de la relación en _index.
	this->RL.erase(RL.begin() + _index);
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//#1: método para diferenciar estructura.
dou INIS_MD::Differentiate(dou **_in_rg)
{
	//---------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------
	//nivel de congruencia de las estructuras.
	dou congruence = 0;

	//---------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------
	//bucle para diferenciar estructura en 'X'.
	for (in i1 = 0; i1 < this->X_LENGTH; i1++)
	{
		//-----------------------------------------------------------------------------
		//-----------------------------------------------------------------------------
		//bucle para diferenciar estructura en 'Y'.
		for (in i2 = 0; i2 < this->Y_LENGTH; i2++)
		{
			//-------------------------------------------------------------------------
			//si "_in_rg" es mayor, entonces.
			if (abs(_in_rg[i1][i2]) > abs(this->IN_RG[i1][i2]))
			{
				//---------------------------------------------------------------------
				//diferenciación de estructuras para obtener congruencia porcentual acumulada.
				congruence += 100.0 - ((abs(_in_rg[i1][i2] - this->IN_RG[i1][i2]) / abs(_in_rg[i1][i2])) * 100.0);
			}

			//-------------------------------------------------------------------------
			//si "_in_rg" es menor o igual, entonces.
			else
			{
				//---------------------------------------------------------------------
				//diferenciación de estructuras para obtener congruencia porcentual acumulada.
				congruence += 100.0 - ((abs(_in_rg[i1][i2] - this->IN_RG[i1][i2]) / abs(this->IN_RG[i1][i2])) * 100.0);
			}
		}
	}

	//---------------------------------------------------------------------------------
	//cálculo del nivel de congruencia.
	congruence = (congruence) / (this->X_LENGTH * this->Y_LENGTH);

	//---------------------------------------------------------------------------------
	//retorno del nivel de congruencia.
	return congruence;
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//#1: método para obtener la cantidad de relaciones.
in INIS_MD::GetRelationAmount()
{
	//---------------------------------------------------------------------------------
	//obtención de la cantidad de relaciones.
	return in(this->RL.size());
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//#2: método para obtener índice de una relación.
in INIS_MD::GetRelationIndex(in _rl_type, in _rl_id, in _upci_type, in _upci_id)
{
	//---------------------------------------------------------------------------------
	//cantidad de relaciones.
	in rl_amount = this->GetRelationAmount();
	
	//---------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------
	//bucle para hallar la relación.
	for (in i1 = 0; i1 < rl_amount; i1++)
	{
		//-----------------------------------------------------------------------------
		//si se halla la relación, entonces retornar el índice de la relación.
		if (_rl_type == abs(this->RL.at(i1).at(0)) && _rl_id == this->RL.at(i1).at(1) && _upci_type == this->RL.at(i1).at(2) && _upci_id == this->RL.at(i1).at(3))	return i1;
	}

	//---------------------------------------------------------------------------------
	//retorno si no se ha hallado la relación.
	return -1;
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//#3: método para obtener índice de la siguiente relación diferencial entrante.
in INIS_MD::GetNextDifferentialInputRelation(in _rl_start_index)
{
	//---------------------------------------------------------------------------------
	//obtención de la cantidad de relaciones.
	in rl_amount = this->GetRelationAmount();

	//---------------------------------------------------------------------------------
	//bucle para obtener el índice de la siguiente relación INDFRLIS entrante.
	for (in i1 = _rl_start_index + 1; i1 < rl_amount; i1++)
	{
		//-----------------------------------------------------------------------------
		//si la relación es entrante, entonces.
		if (this->RL.at(i1).at(0) == -INDFRLIS_MD_TYPE)
		{
			//-------------------------------------------------------------------------
			//retorno del índice de la relación entrante.
			return  i1;
		}
	}

	//---------------------------------------------------------------------------------
	//retorno si no se ha hallado la relación.
	return -1;
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//#4: método especial para obtener índice de la siguiente relación saliente.
in INIS_MD::GetNextOutputRelationS(in _rl_start_index)
{
	//---------------------------------------------------------------------------------
	//obtención de la cantidad de relaciones.
	in rl_amount = this->GetRelationAmount();

	//---------------------------------------------------------------------------------
	//bucle para obtener el índice de la siguiente relación PFRLIS saliente.
	for (in i1 = _rl_start_index + 1; i1 < rl_amount; i1++)
	{
		//-----------------------------------------------------------------------------
		//si la relación es saliente, entonces.
		if (this->RL.at(i1).at(0) == PFRLIS_TYPE)
		{
			//-------------------------------------------------------------------------
			//retorno del índice de la relación saliente.
			return  i1;
		}
	}
	
	//---------------------------------------------------------------------------------
	//bucle para obtener el índice de la siguiente relación INDFRLIS saliente.
	for (in i1 = _rl_start_index + 1; i1 < rl_amount; i1++)
	{
		//-----------------------------------------------------------------------------
		//si la relación es saliente, entonces.
		if (this->RL.at(i1).at(0) == INDFRLIS_MD_TYPE)
		{
			//-------------------------------------------------------------------------
			//retorno del índice de la relación saliente.
			return  i1;
		}
	}
	
	//---------------------------------------------------------------------------------
	//retorno si no se ha hallado la relación.
	return -1;
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//#5: método especial para obtener índice de la anterior relación entrante.
in INIS_MD::GetPreviousInputRelationS(in _rl_start_index)
{
	//---------------------------------------------------------------------------------
	//bucle para obtener el índice de la anterior relación PFRLIS entrante.
	for (in i1 = _rl_start_index - 1; i1 >= 0; i1--)
	{
		//-----------------------------------------------------------------------------
		//si la relación es entrante, entonces.
		if (this->RL.at(i1).at(0) == -PFRLIS_TYPE)
		{
			//-------------------------------------------------------------------------
			//retorno del índice de la relación entrante.
			return  i1;
		}
	}

	//---------------------------------------------------------------------------------
	//bucle para obtener el índice de la anterior relación INDFRLIS entrante.
	for (in i1 = _rl_start_index - 1; i1 >= 0; i1--)
	{
		//-----------------------------------------------------------------------------
		//si la relación es entrante, entonces.
		if (this->RL.at(i1).at(0) == -INDFRLIS_MD_TYPE)
		{
			//-------------------------------------------------------------------------
			//retorno del índice de la relación entrante.
			return  i1;
		}
	}

	//---------------------------------------------------------------------------------
	//retorno si no se ha hallado la relación.
	return -1;
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//#6: método especial para obtener índice de la anterior relación diferencial entrante.
in INIS_MD::GetPreviousDifferentialInputRelation(in _rl_start_index)
{
	//---------------------------------------------------------------------------------
	//bucle para obtener el índice de la anterior relación INDFRLIS entrante.
	for (in i1 = _rl_start_index - 1; i1 >= 0; i1--)
	{
		//-----------------------------------------------------------------------------
		//si la relación es entrante, entonces.
		if (this->RL.at(i1).at(0) == -INDFRLIS_MD_TYPE)
		{
			//-------------------------------------------------------------------------
			//retorno del índice de la relación entrante.
			return  i1;
		}
	}

	//---------------------------------------------------------------------------------
	//retorno si no se ha hallado la relación.
	return -1;
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//#7: método para obtener índice de la última relación diferencial de entradas.
in INIS_MD::GetLastInputDifferentialRelationIndex()
{
	//---------------------------------------------------------------------------------
	//obtención de la cantidad de relaciones.
	in rl_amount = this->GetRelationAmount();

	//---------------------------------------------------------------------------------
	//bucle para obtener el índice de la última relación INDFRLIS entrante.
	for (in i1 = rl_amount - 1; i1 >= 0; i1--)
	{
		//-----------------------------------------------------------------------------
		//si la relación es entrante, entonces.
		if (this->RL.at(i1).at(0) == -INDFRLIS_MD_TYPE)
		{
			//-------------------------------------------------------------------------
			//retorno del índice de la relación entrante.
			return  i1;
		}
	}

	//---------------------------------------------------------------------------------
	//retorno si no se ha hallado la relación.
	return -1;
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//#1: método para setear la estructura.
vo INIS_MD::Set(dou **_input, dou _input_evaluation, in _rs_srg, in _cs_srg, dou _al_srg, dedein &_rl)
{
	//---------------------------------------------------------------------------------
	//paralelización del bucle for.
	#pragma omp parallel for

	//---------------------------------------------------------------------------------
	//seteo del registro de entrada en 'X'.
	for (in i1 = 0; i1 < this->X_LENGTH; i1++)
	{
		//-----------------------------------------------------------------------------
		//paralelización del bucle for.
		#pragma omp parallel for

		//-----------------------------------------------------------------------------
		//seteo del registro de entrada en 'Y'.
		for (in i2 = 0; i2 < this->Y_LENGTH; i2++)
		{
			//-------------------------------------------------------------------------
			//seteo del registro de entrada.
			this->IN_RG[i1][i2] = _input[i1][i2];
		}
	}

	//---------------------------------------------------------------------------------
	//seteo de registros.
	this->EV_RG		= _input_evaluation;													//registro de evaluación.

	//---------------------------------------------------------------------------------
	//seteo de sub-registros.
	this->RS_SRG	= _rs_srg;																//estado de realidad.
	this->CS_SRG	= _cs_srg;																//estado de congruencia.
	this->AL_SRG	= _al_srg;																//nivel de actividad.

	//---------------------------------------------------------------------------------
	//seteo de relaciones.
	this->RL		= _rl;	
}


//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//#1:  método para Clean la estructura.
vo INIS_MD::Clean()
{
	//---------------------------------------------------------------------------------
	//paralelización del bucle for.
	#pragma omp parallel for

	//---------------------------------------------------------------------------------
	//reset del registro de entrada en 'X'.
	for (in i1 = 0; i1 < this->X_LENGTH; i1++)
	{
		//-----------------------------------------------------------------------------
		//paralelización del bucle for.
		#pragma omp parallel for

		//-----------------------------------------------------------------------------
		//reset del registro de entrada en 'Y'.
		for (in i2 = 0; i2 < this->Y_LENGTH; i2++)
		{
			//-------------------------------------------------------------------------
			//reset del registro de diferencia.
			this->IN_RG[i1][i2] = -1;
		}
	}

	//---------------------------------------------------------------------------------
	//reset de registros.
	this->EV_RG		= -1;																	//registro de evaluación.

	//---------------------------------------------------------------------------------
	//reset de sub-registros.	
	this->RS_SRG	= -1;																	//estado de realidad.
	this->CS_SRG	= -1;																	//estado de congruencia.
	this->AL_SRG	= -1;																	//nivel de actividad.

	//---------------------------------------------------------------------------------
	//reset de relaciones.
	this->RL.clear();
}