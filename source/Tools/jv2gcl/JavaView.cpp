// JavaView.cpp: Implementierung der Klasse CJavaView.
//
//////////////////////////////////////////////////////////////////////

#include "JavaView.h"
#include "GCLCInput.h"
#include <cmath>
#include <string>
#include <string.h>

#define X_OFF 70
#define Y_OFF 50


//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

#define ZOOM 10




CJavaView::CJavaView()
{
	AttachNames();
	m_cUnreadChar = '\0';
	m_sParams[0] = '\0';
	m_iNumberOfPoints=0;
	m_iGeometryIndex=0;
}



CJavaView::~CJavaView()
{

}



GReturnValue CJavaView::Import(CGCLCInput *pInput, FILE *hOutput)
{
	m_pInput = pInput;
	m_hOutput = hOutput;

	char *word;
	GReturnValue iRv,err;
	JavaViewcommands com;
	do 
	{
		iRv=GetToken(&word);
		while(!strcmp(word,"<?xml")||!strcmp(word,"<!DOCTYPE"))
		{
			bool skipped=false;
			char c;
			while(!skipped)
			{
				do
				{
					if (m_pInput->ReadChar(&c) != rvG_OK)
						return rvG_InvalidInput;
				}
				while(c!='>');
				skipped=true;
			}
			iRv=GetToken(&word);
		}

		if (iRv!=rvG_EndOfData) 
		{
			m_sParams[0] = '\0';
			com = choose_command(word,m_sParams);
			switch (com)
			{
				case _jv_jvx_model:
					err=ReadModel(); break;

				default:
					return rvG_InvalidInput;
			}
			if (err!=rvG_OK) 
				return err; 
		}
	}
	while (iRv!=rvG_EndOfData);

	return rvG_OK; 
}





GReturnValue CJavaView::ReadModel()
{
	char *word;
	GReturnValue iRv,err;
	JavaViewcommands com;
	do 
	{
		iRv=GetToken(&word);

		while(!strcmp(word,"<meta"))
		{
			bool skipped=false;
			char c;
			while(!skipped)
			{
				do
				{
					if (m_pInput->ReadChar(&c) != rvG_OK)
						return rvG_InvalidInput;
				}
				while(c!='/');
				m_pInput->ReadChar(&c); 
				if (c=='>') 
					skipped=true;
			}
			iRv=GetToken(&word);
		}

		if (!strcmp(word,JVTagName[_jv_jvx_model][1]))
			return rvG_OK;

		const std::string commandname{word};

		if (iRv!=rvG_EndOfData) 
		{
			m_sParams[0] = '\0';
			com = choose_command(word,m_sParams);
			switch (com)
			{
				case _jv_meta: 
				case _jv_version: 
				case _jv_authors: 
				case _jv_description:
				case _jv_title: 
					err=skip(commandname); break;			

				case _jv_geometries: 
					err=ReadGeometries(); break;			

				default:
					return rvG_OK;
			}
			if (err!=rvG_OK) 
				return err; 
		}
	}
	while (iRv!=rvG_EndOfData);

	return rvG_OK; 
}




GReturnValue CJavaView::ReadGeometries()
{
	char *word;
	GReturnValue iRv,err;
	JavaViewcommands com;
	do 
	{
		iRv=GetToken(&word);
		if (!strcmp(word,JVTagName[_jv_geometries][1]))
			return rvG_OK;

		if (iRv!=rvG_EndOfData) 
		{
			m_sParams[0] = '\0';
			com = choose_command(word,m_sParams);
			switch (com)
			{
				case _jv_geometry: 
					err=ReadGeometry(); break;			

				default:
					return rvG_InvalidInput;
			}
			if (err!=rvG_OK) 
				return err; 
		}
	}
	while (iRv!=rvG_EndOfData);

	return rvG_OK; 
}



GReturnValue CJavaView::ReadGeometry()
{
	char *word;
	GReturnValue iRv,err;
	JavaViewcommands com;
	m_iGeometryIndex++;
	m_ListOfFaces.DeleteAll();
	m_ListOfPoints.DeleteAll();
	
	do 
	{
		iRv=GetToken(&word);
		if (!strcmp(word,JVTagName[_jv_geometry][1]))
			return rvG_OK;

		const std::string commandname{word};

		if (iRv!=rvG_EndOfData) 
		{
			m_sParams[0] = '\0';
			com = choose_command(word,m_sParams);
			switch (com)
			{
				case _jv_pointset: 
					err=ReadPointSet(); break;			

				case _jv_lineset: 
					err=ReadLineSet(); break;			

				case _jv_faceset: 
					err=ReadFaceSet(); break;			

				case _jv_vectorfield: 
					err=ReadVectorField(); break;			

				default:
					err=skip(commandname); break;			
			}
			if (err!=rvG_OK) 
				return err; 
		}
	}
	while (iRv!=rvG_EndOfData);

	return rvG_OK; 
}




GReturnValue CJavaView::ReadPointSet()
{
	char *word;
	GReturnValue iRv,err;
	JavaViewcommands com;

	do 
	{
		iRv=GetToken(&word);

		const std::string commandname{word};

		if (!strcmp(word,JVTagName[_jv_pointset][1]))
		{
			struct point *pPoint;
			pPoint=m_ListOfPoints.GetFirstPoint();
			int i=0;
			while (pPoint!=NULL)
			{
				i++;
				if (!((pPoint->r == 0) && 
					  (pPoint->g==0) && 
					  (pPoint->b==0)))
				{
					sprintf(m_sOutput,"color %i %i %i",(int)(pPoint->r),(int)(pPoint->g),(int)(pPoint->b));
					Output(m_sOutput);
				}

				sprintf(m_sOutput,"point P%i_%i %3.2f %3.2f",m_iGeometryIndex, i, pPoint->x, pPoint->y); 
				Output(m_sOutput);
//				sprintf(m_sOutput,"mark P%i_%i",m_iGeometryIndex, i);
//				sprintf(m_sOutput,"cmark P%i_%i",m_iGeometryIndex, i);
//				Output(m_sOutput);

				if (pPoint->name != "")
				{
					sprintf(m_sOutput,"cmark P%i_%i",m_iGeometryIndex, i); 
					Output(m_sOutput);
					sprintf(m_sOutput, "printat_rt P%i_%i {%s}", m_iGeometryIndex, i,
					        pPoint->name.c_str());
					Output(m_sOutput);
				}

				pPoint=m_ListOfPoints.GetNextPoint();
			}
			return rvG_OK; 
		}

		if (iRv!=rvG_EndOfData) 
		{
			m_sParams[0] = '\0';
			com = choose_command(word,m_sParams);
			switch (com)
			{
				case _jv_points: 
					err=ReadPoints(); break;			

				case _jv_p: 
					err=ReadP(); break;			

				case _jv_colors: 
					err=ReadColors(); break;			

				case _jv_thickness: 
					err=ReadThickness(); break;			

				default:
					err=skip(commandname); break;			
			}
			if (err!=rvG_OK) 
				return err; 
		}
	}
	while (iRv!=rvG_EndOfData);

	return rvG_OK;
}




GReturnValue CJavaView::ReadPoints()
{
	char *word;
	GReturnValue iRv,err;
	JavaViewcommands com;
	do 
	{
		iRv=GetToken(&word);
		if (!strcmp(word,JVTagName[_jv_points][1]))
			return rvG_OK;

		const std::string commandname{word};

		if (iRv!=rvG_EndOfData) 
		{
			m_sParams[0] = '\0';
			com = choose_command(word,m_sParams);
			switch (com)
			{
				case _jv_p: 
					err=ReadP(); break;			

				case _jv_thickness: 
					err=ReadThickness(); break;			

				default:
					err=skip(commandname); break;			
			}
			if (err!=rvG_OK) 
				return err; 
		}
	}
	while (iRv!=rvG_EndOfData);
	
	return rvG_OK; 
}




GReturnValue CJavaView::ReadP()
{
	GReturnValue iRv;
	double x,y,z;
	char *word;

	iRv=ReadNumber(&x);
	if (iRv!=rvG_OK)
		return iRv;

	iRv=ReadNumber(&y);
	if (iRv!=rvG_OK)
		return iRv;

//	x/150+2;y/150-14;

	if (!m_ListOfPoints.AddNewPoint(ZOOM*x+X_OFF, ZOOM*y+Y_OFF,0,0,0))
		return rvG_OutOfMemory;

	char* substring=strstr(m_sParams, "name=");
	if(substring!=NULL)
	{
		char* endsubstring=substring+5;
		while(*endsubstring != '"')
			endsubstring++;
		const std::string name{substring + 5,
		                       static_cast<size_t>(endsubstring - substring - 5)};
		m_ListOfPoints.SetLastPointName(name);
	}


	m_iNumberOfPoints++;

	iRv=GetToken(&word);
	if (!strcmp(word,JVTagName[_jv_p][1]))
		return rvG_OK;
	else
	{
		iRv = convert(word, &z);
		if (iRv!=rvG_OK)
			return iRv;
	
		iRv=GetToken(&word);
		if (!strcmp(word,JVTagName[_jv_p][1]))
			return rvG_OK;
		else
			return rvG_InvalidInput;
	}
}




GReturnValue CJavaView::ReadFaceSet()
{
	char *word;
	GReturnValue iRv,err;
	JavaViewcommands com;

	do 
	{
		iRv=GetToken(&word);
		if (!strcmp(word,JVTagName[_jv_faceset][1]))
		{
			CFace *pFace;
			pFace=m_ListOfFaces.GetFirstFace();

			int iNumberOfVerteces=0;

			while (pFace!=NULL)
			{
				if (!((pFace->GetRColor()==0) &&
					(pFace->GetGColor()==0) &&
					(pFace->GetBColor()==0)))
				{
					sprintf(m_sOutput,"color %i %i %i",
						(int)pFace->GetRColor(),
						(int)pFace->GetGColor(),
						(int)pFace->GetBColor());
					Output(m_sOutput);
				}
				
				struct vertex *pVertex;

				pVertex = pFace->GetFirstVertex();
				unsigned int uIndex = pVertex->index;
				unsigned int uFirstIndex = pVertex->index;
				pVertex = pFace->GetNextVertex();

				if (pFace->name != "")
				{
					sprintf(m_sOutput,"translate P%i_label P%i_%i P%i_%i P%i_%i",
						m_ListOfFaces.GetGeometry(),
						m_ListOfFaces.GetGeometry(), uFirstIndex,
						m_ListOfFaces.GetGeometry(), uFirstIndex,
						m_ListOfFaces.GetGeometry(), uFirstIndex);
					Output(m_sOutput);
				}

				while (pVertex != NULL)
				{
					iNumberOfVerteces++;
					sprintf(m_sOutput,"drawsegment P%i_%i P%i_%i",
						m_ListOfFaces.GetGeometry(), uIndex,
						m_ListOfFaces.GetGeometry(), pVertex->index);
					Output(m_sOutput);

					if (pFace->name != "")
					{
						sprintf(m_sOutput,"translate P%i_label P%i_%i P%i_%i P%i_label",
							m_ListOfFaces.GetGeometry(),
							m_ListOfFaces.GetGeometry(), uFirstIndex,
							m_ListOfFaces.GetGeometry(), pVertex->index,
							m_ListOfFaces.GetGeometry());
						Output(m_sOutput);
					}

					uIndex = pVertex->index;
					pVertex = pFace->GetNextVertex();
				}

				sprintf(m_sOutput,"drawsegment P%i_%i P%i_%i",
					m_ListOfFaces.GetGeometry(), uIndex,
					m_ListOfFaces.GetGeometry(), uFirstIndex);
				Output(m_sOutput);

				if (pFace->name != "")
				{
					sprintf(m_sOutput,"towards P%i_label P%i_%i P%i_label %3.2f",
						m_ListOfFaces.GetGeometry(), 
						m_ListOfFaces.GetGeometry(), uFirstIndex,
						m_ListOfFaces.GetGeometry(), 1.0 / (iNumberOfVerteces + 1));
					Output(m_sOutput);

					sprintf(m_sOutput, "printat P%i_label {%s}", m_iGeometryIndex,
					        pFace->name.c_str());
					Output(m_sOutput);
				}

				pFace = m_ListOfFaces.GetNextFace();
			}

			return rvG_OK; 
		}

		const std::string commandname{word};

		if (iRv!=rvG_EndOfData) 
		{
			m_sParams[0] = '\0';
			com = choose_command(word,m_sParams);
			switch (com)
			{
				case _jv_faces: 
					err=ReadFaces(); break;			

				case _jv_f: 
					err=ReadF(); break;			

				case _jv_colors: 
					err=ReadFaceColors(); break;			

				default:
					err=skip(commandname); break;	
			}
			if (err!=rvG_OK) 
				return err; 
		}
	}
	while (iRv!=rvG_EndOfData);

	return rvG_OK;
}





GReturnValue CJavaView::ReadFaces()
{
	char *word;
	GReturnValue iRv,err;
	JavaViewcommands com;
	do 
	{
		iRv=GetToken(&word);
		if (!strcmp(word,JVTagName[_jv_faces][1]))
			return rvG_OK;

		const std::string commandname{word};

		if (iRv!=rvG_EndOfData) 
		{
			m_sParams[0] = '\0';
			com = choose_command(word,m_sParams);
			switch (com)
			{
				case _jv_f: 
					m_ListOfFaces.SetGeometryIndex(m_iGeometryIndex);
					err=ReadF(); break;			

				default:
					err=skip(commandname); break;			
			}
			if (err!=rvG_OK) 
				return err; 
		}
	}
	while (iRv!=rvG_EndOfData);
	
	return rvG_OK; 
}




GReturnValue CJavaView::ReadF()
{
	GReturnValue iRv;
	double number;
	char *word;

	m_ListOfFaces.AddNewFace();

	char* substring=strstr(m_sParams, "name=");
	if(substring!=NULL)
	{
		char* endsubstring=substring+5;
		while(*endsubstring != '"')
			endsubstring++;
		const std::string name{substring + 5,
		                       static_cast<size_t>(endsubstring - substring - 5)};
		m_ListOfFaces.SetLastFaceName(name);
	}


	iRv=GetToken(&word);
	if (iRv!=rvG_OK)
		return rvG_OK;

	while(strcmp(word,JVTagName[_jv_f][1]))
	{
		iRv = convert(word, &number);
		if (iRv!=rvG_OK) 
			return iRv;

		m_ListOfFaces.AddNewVertex((int)number + 1);

		iRv=GetToken(&word);
		if (iRv!=rvG_OK)
			return rvG_OK;

		m_iNumberOfFaces++;
	}

	return rvG_OK;
}





GReturnValue CJavaView::ReadLineSet()
{
	char *word;
	GReturnValue iRv,err;
	JavaViewcommands com;

	do 
	{
		iRv=GetToken(&word);
		if (!strcmp(word,JVTagName[_jv_lineset][1]))
			return rvG_OK; 

		const std::string commandname{word};

		if (iRv!=rvG_EndOfData) 
		{
			m_sParams[0] = '\0';
			com = choose_command(word,m_sParams);
			switch (com)
			{
				case _jv_lines: 
					err=ReadLines(); break;			

				case _jv_l: 
					err=ReadL(); break;			

				default:
					err=skip(commandname); break;	
			}
			if (err!=rvG_OK) 
				return err; 
		}
	}
	while (iRv!=rvG_EndOfData);

	return rvG_OK;
}





GReturnValue CJavaView::ReadLines()
{
	char *word;
	GReturnValue iRv,err;
	JavaViewcommands com;
	do 
	{
		iRv=GetToken(&word);
		if (!strcmp(word,JVTagName[_jv_lines][1]))
			return rvG_OK;

		const std::string commandname{word};

		if (iRv!=rvG_EndOfData) 
		{
			m_sParams[0] = '\0';
			com = choose_command(word,m_sParams);
			switch (com)
			{
				case _jv_l: 
					err=ReadL(); break;			

				default:
					err=skip(commandname); break;	
			}
			if (err!=rvG_OK) 
				return err; 
		}
	}
	while (iRv!=rvG_EndOfData);
	
	return rvG_OK; 
}




GReturnValue CJavaView::ReadL()
{
	GReturnValue iRv;
	double iFirst, iSecond;
	char *word;

	iRv=GetToken(&word);
	if (iRv!=rvG_OK) 
		return iRv;
	iRv = convert(word, &iFirst);
	if (iRv!=rvG_OK) 
		return iRv;

	iRv=GetToken(&word);
	if (iRv!=rvG_OK) 
		return iRv;
	iRv = convert(word, &iSecond);
	if (iRv!=rvG_OK) 
		return iRv;

	sprintf(m_sOutput,"drawdashsegment P%i_%i P%i_%i",m_iGeometryIndex, (int)(iFirst+1), m_iGeometryIndex, (int)(iSecond+1));
	Output(m_sOutput);

	char* substring=strstr(m_sParams, "name=");
	if(substring!=NULL)
	{
		char* endsubstring=substring+5;
		while(*endsubstring != '"')
			endsubstring++;
		const std::string name{substring + 5,
		                       static_cast<size_t>(endsubstring - substring - 5)};

		sprintf(m_sOutput,"midpoint M%i_%i_%i_%i P%i_%i P%i_%i",
			m_iGeometryIndex, (int)(iFirst+1), m_iGeometryIndex, (int)(iSecond+1),
			m_iGeometryIndex, (int)(iFirst+1), m_iGeometryIndex, (int)(iSecond+1));
		Output(m_sOutput);

		sprintf(m_sOutput,"printat_t M%i_%i_%i_%i {%s}",m_iGeometryIndex, (int)(iFirst+1), m_iGeometryIndex, (int)(iSecond+1), name.c_str());
		Output(m_sOutput);
	}


	iRv=GetToken(&word);
	if (iRv!=rvG_OK) 
		return rvG_OK;
	if (!strcmp(word,JVTagName[_jv_l][1]))
		return rvG_OK;
	else
		return rvG_InvalidInput;
}






GReturnValue CJavaView::ReadVectorField()
{
	char *word;
	GReturnValue iRv,err;
	JavaViewcommands com;

	do 
	{
		iRv=GetToken(&word);
		if (!strcmp(word,JVTagName[_jv_vectorfield][1]))
			return rvG_OK; 

		const std::string commandname{word};

		if (iRv!=rvG_EndOfData) 
		{
			m_sParams[0] = '\0';
			com = choose_command(word,m_sParams);
			switch (com)
			{
				case _jv_vectors: 
					err=ReadVectors(); break;			

				case _jv_v: 
					err=ReadV(); break;			

				case _jv_colors: 
					err=skip(commandname); break;			

				default:
					err=skip(commandname); break;			
			}
			if (err!=rvG_OK) 
				return err; 
		}
	}
	while (iRv!=rvG_EndOfData);

	return rvG_OK;
}



GReturnValue CJavaView::ReadVectors()
{
	char *word;
	GReturnValue iRv,err;
	JavaViewcommands com;

	m_iCurrentPoint=0;
	struct point *pPoint;
	pPoint=m_ListOfPoints.GetFirstPoint();
	
	do 
	{
		iRv=GetToken(&word);
		if (!strcmp(word,JVTagName[_jv_vectors][1]))
			return rvG_OK;

		const std::string commandname{word};

		if (iRv!=rvG_EndOfData) 
		{
			m_sParams[0] = '\0';
			com = choose_command(word,m_sParams);
			switch (com)
			{
				case _jv_v: 
					err=ReadV(); break;			

				case _jv_color: 
					err=skip(commandname); break;			

				default:
					err=skip(commandname); break;			
			}
			if (err!=rvG_OK) 
				return err; 
		}
		pPoint=m_ListOfPoints.GetNextPoint();
	}
	while (iRv!=rvG_EndOfData);
	
	return rvG_OK; 
}




GReturnValue CJavaView::ReadV()
{
	GReturnValue iRv;
	double i,j,k;
	char *word;

	iRv=ReadNumber(&i);
	if (iRv!=rvG_OK)
		return iRv;

	iRv=ReadNumber(&j);
	if (iRv!=rvG_OK)
		return iRv;

	iRv=ReadNumber(&k);
	if (iRv!=rvG_OK)
		return iRv;

	m_iCurrentPoint++;

	double x,y;
	m_ListOfPoints.GetCurrentPoint(&x,&y);

	sprintf(m_sOutput,"point Q%i_%i %3.2f %3.2f",
		m_iGeometryIndex, m_iCurrentPoint, x+ZOOM*i, y+ZOOM*j);
	Output(m_sOutput);
	
	sprintf(m_sOutput,"drawvector P%i_%i Q%i_%i",m_iGeometryIndex, 
		m_iCurrentPoint,m_iGeometryIndex, m_iCurrentPoint);
	Output(m_sOutput);

	iRv=GetToken(&word);
	if (!strcmp(word,JVTagName[_jv_v][1]))
		return rvG_OK;
	else
		return rvG_InvalidInput;
}






GReturnValue CJavaView::ReadThickness()
{
	GReturnValue iRv;
	double t;
	char *word;

	iRv=ReadNumber(&t);
	if (iRv!=rvG_OK)
		return iRv;

//	m_pJavaViewPoints[m_iFirstCurrentPoint].thickness = (int)t;

	iRv=GetToken(&word);
	if (!strcmp(word,JVTagName[_jv_thickness][1]))
		return rvG_OK;
	else
		return rvG_InvalidInput;
}





GReturnValue CJavaView::ReadColors()
{
	char *word;
	GReturnValue iRv,err;
	JavaViewcommands com;

	struct point *pPoint;
	pPoint=m_ListOfPoints.GetFirstPoint();

	do 
	{
		iRv=GetToken(&word);
		if (!strcmp(word,JVTagName[_jv_colors][1]))
			return rvG_OK;

		const std::string commandname{word};

		if (iRv!=rvG_EndOfData) 
		{
			m_sParams[0] = '\0';
			com = choose_command(word,m_sParams);
			switch (com)
			{
				case _jv_c: 
					err=ReadC(); break;			
				default:
					err=skip(commandname); break;			
			}
			if (err!=rvG_OK) 
				return err; 
		}
		pPoint=m_ListOfPoints.GetNextPoint();
	}
	while (iRv!=rvG_EndOfData);
	
	return rvG_OK; 
}



GReturnValue CJavaView::ReadFaceColors()
{
	char *word;
	GReturnValue iRv,err;
	JavaViewcommands com;
	
	CFace *pFace;
	pFace=m_ListOfFaces.GetFirstFace();

	do
	{
		iRv=GetToken(&word);
		if (!strcmp(word,JVTagName[_jv_colors][1]))
			return rvG_OK;

		const std::string commandname{word};

		if (iRv!=rvG_EndOfData) 
		{
			m_sParams[0] = '\0';
			com = choose_command(word,m_sParams);
			switch (com)
			{
				case _jv_c: 
					err=ReadFaceC(); break;			
				default:
					err=skip(commandname); break;			
			}
			if (err!=rvG_OK) 
				return err; 
		}
		pFace=m_ListOfFaces.GetNextFace();
	}
	while (iRv!=rvG_EndOfData);
	
	return rvG_OK; 
}




GReturnValue CJavaView::ReadC()
{
	GReturnValue iRv;
	double r,g,b;
	char *word;

	iRv=ReadNumber(&r);
	if (iRv!=rvG_OK)
		return iRv;

	iRv=ReadNumber(&g);
	if (iRv!=rvG_OK)
		return iRv;

	iRv=ReadNumber(&b);
	if (iRv!=rvG_OK)
		return iRv;

	m_ListOfPoints.AttachColorsToCurrentPoint((unsigned char)r,(unsigned char)g,(unsigned char)b);

	iRv=GetToken(&word);
	if (!strcmp(word,JVTagName[_jv_c][1]))
		return rvG_OK;
	else
		return rvG_InvalidInput;

}



GReturnValue CJavaView::ReadFaceC()
{
	GReturnValue iRv;
	double r,g,b;
	char *word;

	iRv=ReadNumber(&r);
	if (iRv!=rvG_OK)
		return iRv;

	iRv=ReadNumber(&g);
	if (iRv!=rvG_OK)
		return iRv;

	iRv=ReadNumber(&b);
	if (iRv!=rvG_OK)
		return iRv;

	m_ListOfFaces.AttachColorsToCurrentFace((unsigned char)r, (unsigned char)g, (unsigned char)b);

	iRv=GetToken(&word);
	if (!strcmp(word,JVTagName[_jv_c][1]))
		return rvG_OK;
	else
		return rvG_InvalidInput;

}




JavaViewcommands CJavaView::choose_command(char* word,char* params)
{
	int i,len, difflen, paramlen=0;
	char *s;
	GReturnValue iRv;
	
	for(i=0;i<(int)_jv_number_of_commands-1;i++)
	{
		//len = strlen(JVTagName[i][0]);
		len = strlen(word);
		
		difflen=strlen(JVTagName[i][0])-len;
		if((difflen>=-1) && (difflen<=1)) 
		{
			if (!strncmp(word,JVTagName[i][0],len) && 
				word[len-1]=='>') 
				return (JavaViewcommands)i;

			if (!strncmp(word,JVTagName[i][0],len)) 
			{
				do
				{
					iRv=GetToken(&s);
					strcpy(params+paramlen,s);
					paramlen=paramlen+strlen(s);
				}
				while(s[strlen(s)-1]!='>');
				return (JavaViewcommands)i;
			}
		}

	}
	return _jv_unsupportedcommand;
}

GReturnValue  CJavaView::skip(const std::string &commandname)
{
	char *word;

	std::string endcommandname{"</" + commandname.substr(1)};
	if (!commandname.empty() && commandname[commandname.size() - 1] != '>')
		endcommandname += ">";

	GReturnValue iRv;
	do 
	{
		iRv=GetToken(&word);
		if (iRv!=rvG_EndOfData) 
		{
			if (word == endcommandname)
				return rvG_OK;
		}
	}
	while (iRv!=rvG_EndOfData);
	return rvG_InvalidInput;
}




GReturnValue CJavaView::GetToken(char **psToken)
{
	char c;
	GReturnValue iRv;
	int i=0;

	*psToken = m_sToken;
	do 
	{
		if (m_cUnreadChar!='\0')
		{
			c=m_cUnreadChar;
			m_cUnreadChar = '\0';
			iRv = rvG_OK;
		}
		else
		{
			iRv = m_pInput->ReadChar(&c); 
		}
		if (iRv==rvG_EndOfData)
			return rvG_EndOfData;
		if (iRv==rvG_InvalidInput)
			return rvG_InvalidInput;

	}         
	while ((c==' ')||(c=='\n')||(c==9));

	m_iLineBeforeLastToken = m_pInput->GetCurrentLine();
	m_iPositionBeforeLastToken = m_pInput->GetCurrentPosition();

	for(;;)
	{
		if (iRv == rvG_EndOfData)
		{
			m_sToken[i]='\0'; 
			return rvG_EndOfData; 
		}
		if ((c==' ')||(c=='\n')||(c==9))
		{ 
			m_sToken[i]='\0'; 
			return rvG_OK; 
		}
		if ((c=='<') && (i>0))
		{ 
			m_cUnreadChar = '<';
			m_sToken[i]='\0'; 
			return rvG_OK; 
		}
		if (c=='>')
		{ 
			m_sToken[i++]='>'; 
			m_sToken[i]='\0'; 
			return rvG_OK; 
		}
		
		if (c=='"')
		{ 
			do
			{
				iRv = m_pInput->ReadChar(&c); 
				m_sToken[i++]=c; 
				if(c=='"')
				{
					m_sToken[i]='\0'; 
					return rvG_OK; 
				}
			}
			while (iRv==rvG_OK);
			return iRv;
		}
		
		m_sToken[i++]=c;
		if (m_cUnreadChar!='\0')
		{
			c=m_cUnreadChar;
			m_cUnreadChar = '\0';
			iRv = rvG_OK;
		}
		else
			iRv = m_pInput->ReadChar(&c); 
	}
}




void CJavaView::AttachNames()
{
	JVTagName[_jv_jvx_model][0] = "<jvx-model>";
	JVTagName[_jv_jvx_model][1] = "</jvx-model>";

	JVTagName[_jv_meta][0] = "<meta>";
	JVTagName[_jv_meta][1] = "</meta>";

	JVTagName[_jv_title][0] = "<title>";
	JVTagName[_jv_title][1] = "</title>";

	JVTagName[_jv_version][0] = "<version>";
	JVTagName[_jv_version][1] = "</version>";

	JVTagName[_jv_authors][0] = "<authors>";
	JVTagName[_jv_authors][1] = "</authors>";

	JVTagName[_jv_description][0] = "<description>";
	JVTagName[_jv_description][1] = "</description>";

	JVTagName[_jv_geometries][0] = "<geometries>";
	JVTagName[_jv_geometries][1] = "</geometries>";

	JVTagName[_jv_geometry][0] = "<geometry>";
	JVTagName[_jv_geometry][1] = "</geometry>";
	
	JVTagName[_jv_pointset][0] = "<pointSet>";
	JVTagName[_jv_pointset][1] = "</pointSet>";

	JVTagName[_jv_points][0] = "<points>";
	JVTagName[_jv_points][1] = "</points>";

	JVTagName[_jv_p][0] = "<p>";
	JVTagName[_jv_p][1] = "</p>";

	JVTagName[_jv_thickness][0] = "<thickness>";
	JVTagName[_jv_thickness][1] = "</thickness>";

	JVTagName[_jv_colors][0] = "<colors>";
	JVTagName[_jv_colors][1] = "</colors>";

	JVTagName[_jv_color][0] = "<color>";
	JVTagName[_jv_color][1] = "</color>";

	JVTagName[_jv_c][0] = "<c>";
	JVTagName[_jv_c][1] = "</c>";

	JVTagName[_jv_vectorfield][0] = "<vectorField>";
	JVTagName[_jv_vectorfield][1] = "</vectorField>";

	JVTagName[_jv_vectors][0] = "<vectors>";
	JVTagName[_jv_vectors][1] = "</vectors>";

	JVTagName[_jv_v][0] = "<v>";
	JVTagName[_jv_v][1] = "</v>";

	JVTagName[_jv_lineset][0] = "<lineSet>";
	JVTagName[_jv_lineset][1] = "</lineSet>";

	JVTagName[_jv_lines][0] = "<lines>";
	JVTagName[_jv_lines][1] = "</lines>";

	JVTagName[_jv_l][0] = "<l>";
	JVTagName[_jv_l][1] = "</l>";

	JVTagName[_jv_faceset][0] = "<faceSet>";
	JVTagName[_jv_faceset][1] = "</faceSet>";

	JVTagName[_jv_faces][0] = "<faces>";
	JVTagName[_jv_faces][1] = "</faces>";

	JVTagName[_jv_f][0] = "<f>";
	JVTagName[_jv_f][1] = "</f>";

}



GReturnValue CJavaView::ReadNumber(double *dNumber)
{
	char* sToken;
	GReturnValue iRv;
	
	iRv=GetToken(&sToken);
	if (iRv!=rvG_OK) 
		return rvG_InvalidInput;
	iRv=convert(sToken,dNumber); 
	if (iRv!=rvG_OK) 
		return rvG_InvalidInput;
	return rvG_OK;
}



GReturnValue CJavaView::convert(char *word, double *number)
{
	int i=0, sign=1;
	double dp=1.00;
	double exp=0;
	bool decimal=false;
	bool exponent=false;
	char c;
	GReturnValue iRv;
	(*number)=0.00;


	if (word[0]=='-') { sign=-1; i++;}

	do
	{
		c=word[i++];
		if (((c<'0')||(c>'9'))&&(c!='.')&&(c!='\0')&&(c!='E')&&(c!='e')) 
			return rvG_InvalidInput;
		if (c!='\0')
		{
			if ((c=='E')||(c=='e'))
			{
				if (exponent==false) 
				{
					decimal=true; 
					iRv=convert(word+i,&exp);
					if (iRv!=rvG_OK)
						return iRv;
					const double mult=pow(10.0,(int)exp);
					(*number)=(*number)*mult;
					return iRv;
				}
				else 
				{ 
					return rvG_InvalidInput; 
				}
			}
			else 
			{
				if (c!='.') 
				{
					if (decimal==false) 
						(*number)=10*(*number)+(double)(c-'0');
					else 
					{ 
						dp=dp*10.00; 
						(*number)=(*number)+((double)(c-'0'))/dp; 
					}
				}
				else 
				{ 
					if (decimal==false) 
						decimal=true; 
					else 
						return rvG_InvalidInput; 
				}
			}
		}               
	}
	while (c!='\0');  

	(*number)=sign*(*number);
	return rvG_OK;
}



GReturnValue CJavaView::Output(char *s)
{
	fwrite("\n",1,1,m_hOutput);
	if (fwrite(s,1,strlen(s),m_hOutput) != strlen(s))
		return rvG_CannotOpenOutputFile;
	else
		return rvG_OK;
}



