/* cfMatObj.H

  object-oriented interface for OptiVec matrix functions of type "fComplex"

  Copyright (c) 1996-2017 by OptiCode
  All Rights Reserved.
*/

#ifndef CFMATOBJ_H

#include <VecObj.h>
#include <cfVecObj.h>
#include <fMatObj.h>
#include <uiVecObj.h>
#include <iVecObj.h>

#define cvta const vector<fComplex>&
#define cmta const matrix<fComplex>&
#define cua  const ui&
#define cuint const ui
#define cta  const fComplex&

#define vta  vector<fComplex>&
#define mta  matrix<fComplex>&
#define ta   fComplex&

#define vt   vector<fComplex>
#define mt   matrix<fComplex>
#define t    fComplex
#define tVector cfVector
#define tMatrix cfMatrix
#define tMatObj cfMatObj
#define tVecObj cfVecObj

#define tbase  float

#ifdef __BORLANDC__
#pragma warn -inl
#endif

#if defined _MSC_VER || (defined __BORLANDC__ && __BORLANDC__ >= 0x500)
namespace OptiVec{
#endif

	VECOBJ_NEW_TEMPLATE class matrix<t>
	{
#ifdef _MSC_VER
	public:
		tMatrix  mx_matr;
		tVector  vx_m0;
		ui       ui_ht, ui_len;
		ui       ui_size;
#else
		tMatrix  mx_matr;
		tVector  vx_m0;
		ui       ui_ht, ui_len;
		ui       ui_size;
		friend class matrix<char>;
		friend class matrix<short>;
		friend class matrix<int>;
		friend class matrix<long>;
		friend class matrix<quad>;
		friend class matrix<unsigned char>;
		friend class matrix<unsigned short>;
		friend class matrix<unsigned int>;
		friend class matrix<unsigned long>;
		#if defined _WIN64
			friend class matrix<uquad>;
		#endif
		friend class matrix<ui>;
		friend class matrix<float>;
		friend class matrix<double>;
		friend class matrix<extended>;
		friend class matrix<fComplex>;
		friend class matrix<dComplex>;
		friend class matrix<eComplex>;
		friend class matrix<fPolar>;
		friend class matrix<dPolar>;
		friend class matrix<ePolar>;
		friend class vector<char>;
		friend class vector<short>;
		friend class vector<int>;
		friend class vector<long>;
		friend class vector<quad>;
		friend class vector<unsigned char>;
		friend class vector<unsigned short>;
		friend class vector<unsigned int>;
		friend class vector<unsigned long>;
		#if defined _WIN64
			friend class vector<uquad>;
		#endif
		friend class vector<ui>;
		friend class vector<float>;
		friend class vector<double>;
		friend class vector<extended>;
		friend class vector<fComplex>;
		friend class vector<dComplex>;
		friend class vector<eComplex>;
#endif
	public:
////////////////////////////////////////////////////////////////////////////////////////////////////
//constructors and destructors
//
		matrix(){ mx_matr=0; vx_m0=0; ui_ht=0; ui_len=0; ui_size=0;}

		matrix(cua ht, cua len)
		{	if((ht > 0) && (len > 0))
			{	mx_matr=MCF_matrix(ht,len); vx_m0=mx_matr[0];
				ui_ht=ht; ui_len=len; ui_size=((ui)ht)*len;
			}
			else{	mx_matr=0; vx_m0=0; ui_ht=0; ui_len=0; ui_size=0;}
		}

		matrix(cua ht, cua len, cta fill)
		{	if(!ht || !len) //size == 0
			{	mx_matr=0; vx_m0=0; ui_ht=0; ui_len=0; ui_size=0;}
			else
			{
				ui_ht=ht; ui_len=len; ui_size=((ui)ht)*len;
				if(!fill.Re && !fill.Im) //fill == 0.0
			{	mx_matr=MCF_matrix0(ht,len); vx_m0=mx_matr[0];}
				else
				{	mx_matr=MCF_matrix(ht,len);  vx_m0=mx_matr[0];
					VCF_equC(vx_m0,ui_size,fill);}
	 		}
		}

		matrix(cmta init)
		{	if(init.ui_size)
			{	mx_matr= MCF_matrix(init.ui_ht, init.ui_len); vx_m0=mx_matr[0];
				ui_ht=init.ui_ht; ui_len=init.ui_len; ui_size=init.ui_size;
				VCF_equV(vx_m0, init.vx_m0, ui_size);
			} else
			{ mx_matr=0; vx_m0=0; ui_ht=0; ui_len=0; ui_size=0;}
		}

		~matrix(){ if(mx_matr) M_free(mx_matr);}


////////////////////////////////////////////////////////////////////////////////////////////////////
//management
//
		void enforceSize()const{ if(!ui_size) throw OptiVec_EmptySizeErr();}
		void enforceSize(ui ht, ui len)const{
			if(ui_len != len || ui_ht != ht || !ui_size) throw OptiVec_SizeErr();}

		void enforceLen(ui len)const{ if(ui_len != len) throw OptiVec_SizeErr();}
		void enforceHt(ui ht)const{ if(ui_ht != ht) throw OptiVec_SizeErr();}

		void enforcePow2()const{ if(!ui_isipow2(ui_len) || !ui_isipow2(ui_ht)) throw OptiVec_Pow2SizeErr();}

		void setSize(cua ht, cua len)
		{	ui size;
			size = ((ui)ht)*len;
			if(!size) dealloc(); //can set size to zero
			if(ui_len != len || ui_ht != ht) //only allocate if different sizes
			{	if(mx_matr) M_free(mx_matr);
				mx_matr= MCF_matrix(ht, len); vx_m0=mx_matr[0];
				ui_len=len; ui_ht=ht; ui_size= size;
			}
		}

		void setSize(cmta X)
		{	if(!X.ui_size) dealloc(); //can set size to zero
			if(ui_len != X.ui_len || ui_ht != X.ui_ht) //only allocate if different sizes
			{	if(mx_matr) M_free(mx_matr);
				mx_matr= MCF_matrix(X.ui_ht, X.ui_len); vx_m0=mx_matr[0];
				ui_len=X.ui_len; ui_ht=X.ui_ht; ui_size= X.ui_size;
			}
		}

		ui getHt()   const { return ui_ht; }
		ui getLen()  const { return ui_len; }
		ui getSize() const { return ui_size; }
		tMatrix getMatrix() const { return mx_matr; }
		tVector getM0()     const { return vx_m0; }

#if !defined (_CRTDBG_MAP_ALLOC) // crtdbg.h defines free as a macro
		void free()
		{	if(mx_matr) M_free(mx_matr);
			mx_matr=0; vx_m0=0; ui_ht=0; ui_len=0; ui_size=0;
		}
#endif

		void dealloc()
		{	if(mx_matr) M_free(mx_matr);
			mx_matr=0; vx_m0=0; ui_ht=0; ui_len=0; ui_size=0;
		}

		ta at(ui m, ui n) const
		{
			if(ui_ht<=m || ui_len<=n) throw OptiVec_SmallSizeErr();
			return mx_matr[m][n];
		}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Overloaded operators
//
		mta operator=(cmta matr)
		{	setSize(matr.ui_ht, matr.ui_len);
			enforceSize();
			VCF_equV(vx_m0, matr.vx_m0, ui_size);
			return *this;}

		mta operator=(cta scalar)
		{	enforceSize();
			VCF_equC(vx_m0, ui_size, scalar);
			return *this;}

		//this is the only really dangerous operator
		tVector operator[](const int& index){return mx_matr[index];}

		mta operator+=(cmta X)
		{
			enforceSize(X.ui_ht, X.ui_len);
			VCF_accV(vx_m0, X.vx_m0, ui_size);
			return *this;}

		mta operator+=(cta scalar)
		{	enforceSize();
			VCF_addC(vx_m0, vx_m0, ui_size, scalar);
			return *this;}

		mta operator*=(cta scalar)
		{	enforceSize();
			VCF_mulC(vx_m0,vx_m0,ui_size,scalar);
			return *this;}

		mt operator+(cmta rhs)
		{	enforceSize(rhs.ui_ht, rhs.ui_len);
			tMatObj tmp(this->ui_ht, this->ui_len);
			VCF_addV(tmp.vx_m0,this->vx_m0,rhs.vx_m0,tmp.ui_size);
			return tmp;}

		mt operator+(cta C)
		{	tMatObj tmp(this->ui_ht, this->ui_len);
			tmp.enforceSize();
			VCF_addC(tmp.vx_m0,this->vx_m0,tmp.ui_size,C);
			return tmp;}

		mt operator-(cmta rhs)
		{	enforceSize(rhs.ui_ht, rhs.ui_len);
			tMatObj tmp(this->ui_ht, this->ui_len);
			VCF_subV(tmp.vx_m0,this->vx_m0,rhs.vx_m0,tmp.ui_size);
			return tmp;}

		mt operator-(cta C)
		{	tMatObj tmp(this->ui_ht, this->ui_len);
			tmp.enforceSize();
			VCF_subC(tmp.vx_m0,this->vx_m0,tmp.ui_size,C);
			return tmp;}

		mt operator*(cmta rhs)
		{	enforceLen(rhs.ui_ht);
			tMatObj tmp(this->ui_ht, rhs.ui_len);
			tmp.enforceSize();
			MCFmulM(tmp.vx_m0,this->vx_m0,rhs.vx_m0,this->ui_ht, this->ui_len,rhs.ui_len);
			return tmp;}

		vt operator*(cvta rhs)
		{	enforceLen(rhs.ui_size);
			tVecObj tmp(this->ui_ht);
			tmp.enforceSize();
			MCFmulV(tmp.vx_vect,this->vx_m0,rhs.vx_vect,this->ui_ht, this->ui_len);
			return tmp;}

		// operator*(cvta VX, cmta MY) is member of cfVecObj, see below

		mt operator*(cta C)
		{	tMatObj tmp(this->ui_ht, this->ui_len);
			tmp.enforceSize();
			VCF_mulC(tmp.vx_m0,this->vx_m0,tmp.ui_size,C);
			return tmp;}

		mt operator/(cta C)
		{	tMatObj tmp(this->ui_ht, this->ui_len);
			tmp.enforceSize();
			VCF_divC(tmp.vx_m0,this->vx_m0,tmp.ui_size,C);
			return tmp;}

		friend mt operator+(cta C, cmta X)
		{	tMatObj tmp(X.ui_ht,X.ui_len);
			VCF_addC(tmp.vx_m0,X.vx_m0,tmp.ui_size,C);
			return tmp;}

		friend mt operator-(cta C, cmta X)
		{	tMatObj tmp(X.ui_ht,X.ui_len);
			VCF_subrC(tmp.vx_m0,X.vx_m0,tmp.ui_size,C);
			return tmp;}

		friend mt operator*(cta C, cmta X)
		{	tMatObj tmp(X.ui_ht,X.ui_len);
			VCF_mulC(tmp.vx_m0,X.vx_m0,tmp.ui_size,C);
			return tmp;}

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//OptiVec functions in member format
//from MCXstd.h
//

/////////////////////////////////////////////////////////////////////////////////////////
//Generation
//
		//these are not recommended, better to use constructors
		void _matrix(cuint ht, cuint len) //underscore necessary to avoid confusing with constructor
		{
			dealloc();
			ui_size=((ui)ht)*len;
			if(ui_size){ ui_ht=ht;ui_len=len;mx_matr= MCF_matrix(ht,len);vx_m0=mx_matr[0];}
		}

		void _matrix0(cuint ht, cuint len) //underscore necessary to avoid confusing with constructor
		{
			dealloc();
			ui_size=((ui)ht)*len;
			if(ui_size){ ui_ht=ht;ui_len=len;mx_matr= MCF_matrix0(ht,len);vx_m0=mx_matr[0];}
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Addressing single matrix elements
//
		t* Pelement(cuint m, cuint n) const
		{
			if(ui_ht<=m || ui_len<=n) throw OptiVec_SmallSizeErr();
			return MCFPelement(vx_m0,ui_ht,ui_len,m,n);
		}

		t element(cuint m, cuint n) const
		{
			if(ui_ht<=m || ui_len<=n) throw OptiVec_SmallSizeErr();
			return MCFelement(vx_m0,ui_ht,ui_len,m,n);
		}

		void getElement(t *y, cuint m, cuint n)
		{
			if(ui_ht<=m || ui_len<=n) throw OptiVec_SmallSizeErr();
			return MCFgetElement(y,vx_m0,ui_ht,ui_len,m,n);
		}

		void setElement(cuint m, cuint n, cta C)
		{
			if(ui_ht<=m || ui_len<=n) throw OptiVec_SmallSizeErr();
			MCFsetElement(vx_m0,ui_ht,ui_len,m,n,C);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Initialization
//

		void equ0()
		{
			enforceSize();
			VCF_equ0(vx_m0,ui_size);
		}

		void equ1()
		{
			enforceSize();
			MCFequ1(vx_m0,ui_len);
		}

		void equm1()
		{
			enforceSize();
			MCFequm1(vx_m0,ui_len);
		}

		long random(const long& seed, cta Min, cta Max)
		{
			enforceSize();
			return VCF_random(vx_m0, ui_size, seed, Min, Max);
		}

		void randomLC(const long& seed, cta Min, cta Max, V_RANDOMLCSTATE *state)
		{
			enforceSize();
			VCF_randomLC(vx_m0, ui_size, seed, Min, Max, state);
		}

		void outerprod(cvta X, cvta Y)
		{
			enforceSize( X.ui_size, Y.ui_size);
			MCFouterprod(vx_m0, X.vx_vect, Y.vx_vect, ui_ht, ui_len);
		}

		void Row_equ0(cuint iRow)
		{
			if( !ui_len || (ui_ht <= iRow) )throw OptiVec_SmallSizeErr();
			MCFRow_equ0(vx_m0, ui_ht, ui_len, iRow);
		}

		void Col_equ0(cuint iCol)
		{
			if( !ui_ht || (ui_len <= iCol) )throw OptiVec_SmallSizeErr();
			MCFCol_equ0(vx_m0, ui_ht, ui_len, iCol);
		}

		void Dia_equ0()
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SmallSizeErr();
			MCFDia_equ0(vx_m0, ui_len);
		}

		void Row_equC(cuint iRow, cta C)
		{
			if( !ui_len || (ui_ht <= iRow) )throw OptiVec_SmallSizeErr();
			MCFRow_equC(vx_m0, ui_ht, ui_len, iRow, C);
		}

		void Col_equC(cuint iCol, cta C)
		{
			if( !ui_ht || (ui_len <= iCol) )throw OptiVec_SmallSizeErr();
			MCFCol_equC(vx_m0, ui_ht, ui_len, iCol, C);
		}

		void Dia_equC(cta C)
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SmallSizeErr();
			MCFDia_equC(vx_m0, ui_len, C);
		}

		void Row_equV(cuint iRow, cvta X)
		{
			if( !ui_len || (ui_ht <= iRow) )throw OptiVec_SmallSizeErr();
			MCFRow_equV(vx_m0, ui_ht, ui_len, iRow, X.vx_vect);
		}

		void Col_equV(cuint iCol, cvta X)
		{
			if( !ui_ht || (ui_len <= iCol) )throw OptiVec_SmallSizeErr();
			MCFCol_equV(vx_m0, ui_ht, ui_len, iCol, X.vx_vect);
		}

		void Dia_equV(cvta X)
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SmallSizeErr();
			MCFDia_equV(vx_m0, ui_len, X.vx_vect);
		}

		void Trd_equM(cmta MTrd)
		{
			if( MTrd.ui_ht != 3 || ui_ht != ui_len || ui_len != MTrd.ui_len )throw OptiVec_SizeErr();
			MCFTrd_equM(vx_m0, MTrd.vx_m0, ui_len);
		}

		void Trd_extract(cmta MA)
		{
			if( ui_ht != 3 || MA.ui_ht != MA.ui_len || MA.ui_len != ui_len )throw OptiVec_SizeErr();
			MCFTrd_extract(vx_m0, MA.vx_m0, ui_len);
		}

		void equM(cmta X)
		{
			enforceSize(X.ui_ht, X.ui_len);
			MCFequM(vx_m0, X.vx_m0, ui_ht, ui_len);
		}

		void neg(cmta X)
		{
			enforceSize(X.ui_ht, X.ui_len);
			MCFneg(vx_m0, X.vx_m0, ui_ht, ui_len);
		}

		void conj(cmta X)
		{
			enforceSize(X.ui_ht, X.ui_len);
			MCFconj(vx_m0, X.vx_m0, ui_ht, ui_len);
		}

		void UequL()
		{
			if( !ui_len || ui_len != ui_ht )throw OptiVec_SizeErr();
			MCFUequL(vx_m0, ui_len);
		}

		void LequU()
		{
			if( !ui_len || ui_len != ui_ht )throw OptiVec_SizeErr();
			MCFLequU(vx_m0, ui_len);
		}

/////////////////////////////////////////////////////////////////////////////////////////
//Data-type interconversions
//
		//defined by including cdMatObj.h
		void CDtoCF(const cdMatObj& X);

		//defined by including ceMatObj.h
		void CEtoCF(const ceMatObj& X);



/////////////////////////////////////////////////////////////////////////////////////////
//Submatrices
//
		void submatrix(cmta MSrce, cuint firstRowInCol, cuint sampInCol, cuint firstColInRow, cuint sampInRow )
		{
			if( (MSrce.ui_ht  <= firstRowInCol+(ui_ht-1) *sampInCol) ||
	 	 	  (MSrce.ui_len <= firstColInRow+(ui_len-1)*sampInRow) ) throw OptiVec_SizeErr();
			MCFsubmatrix(vx_m0, ui_ht, ui_len, MSrce.vx_m0, MSrce.ui_ht, MSrce.ui_len,
	 	 	   firstRowInCol, sampInCol, firstColInRow, sampInRow );
		}

		void submatrix_equM(cuint firstRowInCol, cuint sampInCol, cuint firstColInRow, cuint sampInRow, cmta MSrce)
		{
			if( (ui_ht  <= firstRowInCol+(MSrce.ui_ht-1) *sampInCol) ||
	 	 	  (ui_len <= firstColInRow+(MSrce.ui_len-1)*sampInRow) ) throw OptiVec_SizeErr();
			MCFsubmatrix_equM(vx_m0, ui_ht, ui_len,
	 	 	   firstRowInCol, sampInCol, firstColInRow, sampInRow,
	 	 	   MSrce.vx_m0, MSrce.ui_ht, MSrce.ui_len );
		}

		void equMblock(cmta MSrce, cuint firstRowInCol, cuint firstColInRow )
		{
			if( (MSrce.ui_ht  < firstRowInCol+ui_ht) ||
	 	 	  (MSrce.ui_len < firstColInRow+ui_len) ) throw OptiVec_SizeErr();
			MCFequMblock(vx_m0, ui_ht, ui_len, MSrce.vx_m0, MSrce.ui_ht, MSrce.ui_len,
	 	 	   firstRowInCol, firstColInRow );
		}

		void equMblockT(cmta MSrce, cuint firstRowInCol, cuint firstColInRow )
		{
			if( (MSrce.ui_ht  < firstRowInCol+ui_len) ||
	 	 	  (MSrce.ui_len < firstColInRow+ui_ht) ) throw OptiVec_SizeErr();
			MCFequMblockT(vx_m0, ui_ht, ui_len, MSrce.vx_m0, MSrce.ui_ht, MSrce.ui_len,
	 	 	   firstRowInCol, firstColInRow );
		}

		void block_equM(cuint firstRowInCol, cuint firstColInRow, cmta MSrce )
		{
			if( (ui_ht  < firstRowInCol+MSrce.ui_ht) ||
	 	 	  (ui_len < firstColInRow+MSrce.ui_len) ) throw OptiVec_SizeErr();
			MCFblock_equM(vx_m0, ui_ht, ui_len, firstRowInCol, firstColInRow,
	 	 	   MSrce.vx_m0, MSrce.ui_ht, MSrce.ui_len );
		}

		void block_equMT(cuint firstRowInCol, cuint firstColInRow, cmta MSrce )
		{
			if( (ui_ht  < firstRowInCol+MSrce.ui_len) ||
	 	 	  (ui_len < firstColInRow+MSrce.ui_ht) ) throw OptiVec_SizeErr();
			MCFblock_equMT(vx_m0, ui_ht, ui_len, firstRowInCol, firstColInRow,
	 	 	   MSrce.vx_m0, MSrce.ui_ht, MSrce.ui_len );
		}

/////////////////////////////////////////////////////////////////////////////////////////
// Deleting or inserting a row or a column
//

		void Row_delete(cmta MA, cuint iRow)
		{
			if( MA.ui_ht <= iRow ) throw OptiVec_SmallSizeErr();
			if( mx_matr == MA.mx_matr ) // overwrite MA?
			{	ui_size -= ui_len;  // don't reallocate, as row pointers remain intact
				MCFRow_delete(vx_m0, vx_m0, ui_ht--, ui_len, iRow);
			}
			else
			{	enforceSize(MA.ui_ht-1, MA.ui_len);
				MCFRow_delete(vx_m0, MA.vx_m0, MA.ui_ht, MA.ui_len, iRow);
			}
		}

		void Col_delete(cmta MA, cuint iCol)
		{
			if( MA.ui_len <= iCol ) throw OptiVec_SmallSizeErr();
			if( mx_matr == MA.mx_matr ) // overwrite MA?
			{
				tMatrix tmp = MCF_matrix( ui_ht, ui_len-1 );
				MCFCol_delete(tmp[0], MA.vx_m0, ui_ht, ui_len--, iCol);
				M_free( mx_matr );
				vx_m0 = tmp[0];
				ui_size -= ui_ht;
				mx_matr = tmp;
			}
			else
			{
				enforceSize(MA.ui_ht, MA.ui_len-1);
				MCFCol_delete(vx_m0, MA.vx_m0, ui_ht, ui_len, iCol);
			}
		}

		void Row_insert(cmta MA, cuint iRow, cvta X)
		{
			if( ui_ht <= iRow ) throw OptiVec_SmallSizeErr();
			if( mx_matr == MA.mx_matr ) // overwrite MA?
			{
				tMatrix tmp = MCF_matrix( ++ui_ht, ui_len );
				MCFRow_insert(tmp[0], MA.vx_m0, ui_ht, ui_len, iRow, X.vx_vect);
				M_free( mx_matr );
				vx_m0 = tmp[0];
				ui_size += ui_len;
				mx_matr = tmp;
			}
			else
			{
				enforceSize(MA.ui_ht+1, MA.ui_len);
				MCFRow_insert(vx_m0, MA.vx_m0, ui_ht, ui_len, iRow, X.vx_vect);
			}
		}

		void Col_insert(cmta MA, cuint iCol, cvta X)
		{
			if( ui_len <= iCol ) throw OptiVec_SmallSizeErr();
			if( mx_matr == MA.mx_matr ) // overwrite MA?
			{
				tMatrix tmp = MCF_matrix( ui_ht, ++ui_len );
				MCFCol_insert(tmp[0], MA.vx_m0, ui_ht, ui_len, iCol, X.vx_vect);
				M_free( mx_matr );
				vx_m0 = tmp[0];
				ui_size += ui_ht;
				mx_matr = tmp;
			}
			else
			{
				enforceSize(MA.ui_ht, MA.ui_len+1);
				MCFCol_insert(vx_m0, MA.vx_m0, ui_ht, ui_len, iCol, X.vx_vect);
			}
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Basic arithmetic operations performed on one single row, or one single column
// of any matrix, or on the diagonal of a square matrix
//

		void Row_neg(cuint iRow)
		{
			if( !ui_len || (ui_ht <= iRow) )throw OptiVec_SmallSizeErr();
			MCFRow_neg(vx_m0, ui_ht, ui_len, iRow);
		}

		void Col_neg(cuint iCol)
		{
			if( !ui_ht || (ui_len <= iCol) )throw OptiVec_SmallSizeErr();
			MCFCol_neg(vx_m0, ui_ht, ui_len, iCol);
		}

		void Row_conj(cuint iRow)
		{
			if( !ui_len || (ui_ht <= iRow) )throw OptiVec_SmallSizeErr();
			MCFRow_conj(vx_m0, ui_ht, ui_len, iRow);
		}

		void Col_conj(cuint iCol)
		{
			if( !ui_ht || (ui_len <= iCol) )throw OptiVec_SmallSizeErr();
			MCFCol_conj(vx_m0, ui_ht, ui_len, iCol);
		}

		void Row_addC(cuint iRow, cta C)
		{
			if( !ui_len || (ui_ht <= iRow) )throw OptiVec_SmallSizeErr();
			MCFRow_addC(vx_m0, ui_ht, ui_len, iRow, C);
		}

		void Col_addC(cuint iCol, cta C)
		{
			if( !ui_ht || (ui_len <= iCol) )throw OptiVec_SmallSizeErr();
			MCFCol_addC(vx_m0, ui_ht, ui_len, iCol, C);
		}

		void Dia_addC(cta C)
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SmallSizeErr();
			MCFDia_addC(vx_m0, ui_len, C);
		}

		void Row_addV(cuint iRow, cvta X)
		{
			if( !ui_len || (ui_ht <= iRow) || (X.ui_size < ui_len))throw OptiVec_SmallSizeErr();
			MCFRow_addV(vx_m0, ui_ht, ui_len, iRow, X.vx_vect);
		}

		void Col_addV(cuint iCol, cvta X)
		{
			if( !ui_ht || (ui_len <= iCol) || (X.ui_size < ui_ht) )throw OptiVec_SmallSizeErr();
			MCFCol_addV(vx_m0, ui_ht, ui_len, iCol, X.vx_vect);
		}

		void Dia_addV(cvta X)
		{
			if( !ui_len || (ui_len != ui_ht) || (X.ui_size < ui_len) )throw OptiVec_SmallSizeErr();
			MCFDia_addV(vx_m0, ui_len, X.vx_vect);
		}

		void Row_subC(cuint iRow, cta C)
		{
			if( !ui_len || (ui_ht <= iRow) )throw OptiVec_SmallSizeErr();
			MCFRow_subC(vx_m0, ui_ht, ui_len, iRow, C);
		}

		void Col_subC(cuint iCol, cta C)
		{
			if( !ui_ht || (ui_len <= iCol) )throw OptiVec_SmallSizeErr();
			MCFCol_subC(vx_m0, ui_ht, ui_len, iCol, C);
		}

		void Dia_subC(cta C)
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SmallSizeErr();
			MCFDia_subC(vx_m0, ui_len, C);
		}

		void Row_subV(cuint iRow, cvta X)
		{
			if( !ui_len || (ui_ht <= iRow) || (X.ui_size < ui_len))throw OptiVec_SmallSizeErr();
			MCFRow_subV(vx_m0, ui_ht, ui_len, iRow, X.vx_vect);
		}

		void Col_subV(cuint iCol, cvta X)
		{
			if( !ui_ht || (ui_len <= iCol) || (X.ui_size < ui_ht) )throw OptiVec_SmallSizeErr();
			MCFCol_subV(vx_m0, ui_ht, ui_len, iCol, X.vx_vect);
		}

		void Dia_subV(cvta X)
		{
			if( !ui_len || (ui_len != ui_ht) || (X.ui_size < ui_len) )throw OptiVec_SmallSizeErr();
			MCFDia_subV(vx_m0, ui_len, X.vx_vect);
		}

		void Row_subrC(cuint iRow, cta C)
		{
			if( !ui_len || (ui_ht <= iRow) )throw OptiVec_SmallSizeErr();
			MCFRow_subrC(vx_m0, ui_ht, ui_len, iRow, C);
		}

		void Col_subrC(cuint iCol, cta C)
		{
			if( !ui_ht || (ui_len <= iCol) )throw OptiVec_SmallSizeErr();
			MCFCol_subrC(vx_m0, ui_ht, ui_len, iCol, C);
		}

	 	void Dia_subrC(cta C)
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SmallSizeErr();
			MCFDia_subrC(vx_m0, ui_len, C);
		}

		void Row_subrV(cuint iRow, cvta X)
		{
			if( !ui_len || (ui_ht <= iRow) || (X.ui_size < ui_len))throw OptiVec_SmallSizeErr();
			MCFRow_subrV(vx_m0, ui_ht, ui_len, iRow, X.vx_vect);
		}

		void Col_subrV(cuint iCol, cvta X)
		{
			if( !ui_ht || (ui_len <= iCol) || (X.ui_size < ui_ht) )throw OptiVec_SmallSizeErr();
			MCFCol_subrV(vx_m0, ui_ht, ui_len, iCol, X.vx_vect);
		}

	 	void Dia_subrV(cvta X)
		{
			if( !ui_len || (ui_len != ui_ht) || (X.ui_size < ui_len) )throw OptiVec_SmallSizeErr();
			MCFDia_subrV(vx_m0, ui_len, X.vx_vect);
		}

		void Row_mulC(cuint iRow, cta C)
		{
			if( !ui_len || (ui_ht <= iRow) )throw OptiVec_SmallSizeErr();
			MCFRow_mulC(vx_m0, ui_ht, ui_len, iRow, C);
		}

		void Col_mulC(cuint iCol, cta C)
		{
			if( !ui_ht || (ui_len <= iCol) )throw OptiVec_SmallSizeErr();
			MCFCol_mulC(vx_m0, ui_ht, ui_len, iCol, C);
		}

	 	void Dia_mulC(cta C)
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SmallSizeErr();
			MCFDia_mulC(vx_m0, ui_len, C);
		}

		void Row_mulV(cuint iRow, cvta X)
		{
			if( !ui_len || (ui_ht <= iRow) || (X.ui_size < ui_len))throw OptiVec_SmallSizeErr();
			MCFRow_mulV(vx_m0, ui_ht, ui_len, iRow, X.vx_vect);
		}

		void Col_mulV(cuint iCol, cvta X)
		{
			if( !ui_ht || (ui_len <= iCol) || (X.ui_size < ui_ht) )throw OptiVec_SmallSizeErr();
			MCFCol_mulV(vx_m0, ui_ht, ui_len, iCol, X.vx_vect);
		}

	 	void Dia_mulV(cvta X)
		{
			if( !ui_len || (ui_len != ui_ht) || (X.ui_size < ui_len) )throw OptiVec_SmallSizeErr();
			MCFDia_mulV(vx_m0, ui_len, X.vx_vect);
		}

		void Row_divC(cuint iRow, cta C)
		{
			if( !ui_len || (ui_ht <= iRow) )throw OptiVec_SmallSizeErr();
			MCFRow_divC(vx_m0, ui_ht, ui_len, iRow, C);
		}

		void Col_divC(cuint iCol, cta C)
		{
			if( !ui_ht || (ui_len <= iCol) )throw OptiVec_SmallSizeErr();
			MCFCol_divC(vx_m0, ui_ht, ui_len, iCol, C);
		}

	 	void Dia_divC(cta C)
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SmallSizeErr();
			MCFDia_divC(vx_m0, ui_len, C);
		}

		void Row_divV(cuint iRow, cvta X)
		{
			if( !ui_len || (ui_ht <= iRow) || (X.ui_size < ui_len))throw OptiVec_SmallSizeErr();
			MCFRow_divV(vx_m0, ui_ht, ui_len, iRow, X.vx_vect);
		}

		void Col_divV(cuint iCol, cvta X)
		{
			if( !ui_ht || (ui_len <= iCol) || (X.ui_size < ui_ht) )throw OptiVec_SmallSizeErr();
			MCFCol_divV(vx_m0, ui_ht, ui_len, iCol, X.vx_vect);
		}

	 	void Dia_divV(cvta X)
		{
			if( !ui_len || (ui_len != ui_ht) || (X.ui_size < ui_len) )throw OptiVec_SmallSizeErr();
			MCFDia_divV(vx_m0, ui_len, X.vx_vect);
		}

		void Row_divrC(cuint iRow, cta C)
		{
			if( !ui_len || (ui_ht <= iRow) )throw OptiVec_SmallSizeErr();
			MCFRow_divrC(vx_m0, ui_ht, ui_len, iRow, C);
		}

		void Col_divrC(cuint iCol, cta C)
		{
			if( !ui_ht || (ui_len <= iCol) )throw OptiVec_SmallSizeErr();
			MCFCol_divrC(vx_m0, ui_ht, ui_len, iCol, C);
		}

	 	void Dia_divrC(cta C)
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SmallSizeErr();
			MCFDia_divrC(vx_m0, ui_len, C);
		}

		void Row_divrV(cuint iRow, cvta X)
		{
			if( !ui_len || (ui_ht <= iRow) || (X.ui_size < ui_len))throw OptiVec_SmallSizeErr();
			MCFRow_divrV(vx_m0, ui_ht, ui_len, iRow, X.vx_vect);
		}

		void Col_divrV(cuint iCol, cvta X)
		{
			if( !ui_ht || (ui_len <= iCol) || (X.ui_size < ui_ht) )throw OptiVec_SmallSizeErr();
			MCFCol_divrV(vx_m0, ui_ht, ui_len, iCol, X.vx_vect);
		}

	 	void Dia_divrV(cvta X)
		{
			if( !ui_len || (ui_len != ui_ht) || (X.ui_size < ui_len) )throw OptiVec_SmallSizeErr();
			MCFDia_divrV(vx_m0, ui_len, X.vx_vect);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//One-dimensional vector operations
//performed along all rows or columns, or along the diagonal of a square matrix
//(some of these functions belong to fVecObj.h)

		t Dia_maxReIm() const
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SizeErr();
			return MCFDia_maxReIm(vx_m0, ui_len);
		}

		t Dia_minReIm() const
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SizeErr();
			return MCFDia_minReIm(vx_m0, ui_len);
		}

		tbase Dia_absmax() const
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SizeErr();
			return MCFDia_absmax(vx_m0, ui_len);
		}

		tbase Dia_absmin() const
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SizeErr();
			return MCFDia_absmin(vx_m0, ui_len);
		}

		t Dia_absmaxReIm() const
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SizeErr();
			return MCFDia_absmaxReIm(vx_m0, ui_len);
		}

		t Dia_absminReIm() const
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SizeErr();
			return MCFDia_absminReIm(vx_m0, ui_len);
		}

		t Dia_sabsmax() const
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SizeErr();
			return MCFDia_sabsmax(vx_m0, ui_len);
		}

		t Dia_sabsmin() const
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SizeErr();
			return MCFDia_sabsmin(vx_m0, ui_len);
		}

		t Dia_cabsmax() const
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SizeErr();
			return MCFDia_cabsmax(vx_m0, ui_len);
		}

		t Dia_cabsmin() const
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SizeErr();
			return MCFDia_cabsmin(vx_m0, ui_len);
		}

 		t Dia_sum() const
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SizeErr();
			return MCFDia_sum(vx_m0, ui_len);
		}

		t Dia_prod() const
		{
			if( !ui_len || (ui_len != ui_ht) )throw OptiVec_SizeErr();
			return MCFDia_prod(vx_m0, ui_len);
		}

		void Rows_runsum()
		{
			enforceSize();
			MCFRows_runsum(vx_m0, ui_ht, ui_len);
		}

		void Cols_runsum()
		{
			enforceSize();
			MCFCols_runsum(vx_m0, ui_ht, ui_len);
		}

		void Rows_runprod()
		{
			enforceSize();
			MCFRows_runprod(vx_m0, ui_ht, ui_len);
		}

		void Cols_runprod()
		{
			enforceSize();
			MCFCols_runprod(vx_m0, ui_ht, ui_len);
		}

		void Rows_rotate(const ssize_t pos)
		{
			enforceSize();
			MCFRows_rotate(vx_m0, ui_ht, ui_len, pos);
		}

		void Cols_rotate(const ssize_t pos)
		{
			enforceSize();
			MCFCols_rotate(vx_m0, ui_ht, ui_len, pos);
		}

		void Rows_rotate_buf(const ssize_t pos, cmta MBuf)
		{
			enforceSize();
			MCFRows_rotate_buf(vx_m0, ui_ht, ui_len, pos, MBuf.vx_m0);
		}

		void Cols_rotate_buf(const ssize_t pos, cmta MBuf)
		{
			enforceSize();
			MCFCols_rotate_buf(vx_m0, ui_ht, ui_len, pos, MBuf.vx_m0);
		}

		void Rows_reflect()
		{
			enforceSize();
			MCFRows_reflect(vx_m0, ui_ht, ui_len);
		}

		void Cols_reflect()
		{
			enforceSize();
			MCFCols_reflect(vx_m0, ui_ht, ui_len);
		}

		void Rows_rev()
		{
			enforceSize();
			MCFRows_rev(vx_m0, ui_ht, ui_len);
		}

		void Cols_rev()
		{
			enforceSize();
			MCFCols_rev(vx_m0, ui_ht, ui_len);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Operations involving two rows or two colums of one matrix
//
		void Rows_exchange(cuint row1, cuint row2)
		{
			if( !ui_len || (ui_ht <= row1) || (ui_ht <= row2))throw OptiVec_SizeErr();
			MCFRows_exchange(vx_m0, ui_ht, ui_len, row1, row2);
		}

		void Cols_exchange(cuint col1, cuint col2)
		{
			if( !ui_len || (ui_ht <= col1) || (ui_ht <= col2))throw OptiVec_SizeErr();
			MCFCols_exchange(vx_m0, ui_ht, ui_len, col1, col2);
		}

		void Rows_add(cuint destRow, cuint srceRow)
		{
			if( !ui_len || (ui_ht <= destRow) || (ui_ht <= srceRow))throw OptiVec_SizeErr();
			MCFRows_add(vx_m0, ui_ht, ui_len, destRow, srceRow);
		}

		void Cols_add(cuint destCol, cuint srceCol)
		{
			if( !ui_len || (ui_ht <= destCol) || (ui_ht <= srceCol))throw OptiVec_SizeErr();
			MCFCols_add(vx_m0, ui_ht, ui_len, destCol, srceCol);
		}

		void Rows_sub(cuint destRow, cuint srceRow)
		{
			if( !ui_len || (ui_ht <= destRow) || (ui_ht <= srceRow))throw OptiVec_SizeErr();
			MCFRows_sub(vx_m0, ui_ht, ui_len, destRow, srceRow);
		}

		void Cols_sub(cuint destCol, cuint srceCol)
		{
			if( !ui_len || (ui_ht <= destCol) || (ui_ht <= srceCol))throw OptiVec_SizeErr();
			MCFCols_sub(vx_m0, ui_ht, ui_len, destCol, srceCol);
		}

		void Rows_Cadd(cuint destRow, cuint srceRow, cta C)
		{
			if( !ui_len || (ui_ht <= destRow) || (ui_ht <= srceRow))throw OptiVec_SizeErr();
			MCFRows_Cadd(vx_m0, ui_ht, ui_len, destRow, srceRow, C);
		}

		void Cols_Cadd(cuint destCol, cuint srceCol, cta C)
		{
			if( !ui_len || (ui_ht <= destCol) || (ui_ht <= srceCol))throw OptiVec_SizeErr();
			MCFCols_Cadd(vx_m0, ui_ht, ui_len, destCol, srceCol, C);
		}

		void Rows_lincomb(cuint destRow, cta destC, cuint srceRow, cta srceC)
		{
			if( !ui_len || (ui_ht <= destRow) || (ui_ht <= srceRow))throw OptiVec_SizeErr();
			MCFRows_lincomb(vx_m0, ui_ht, ui_len, destRow, destC, srceRow, srceC);
		}

		void Cols_lincomb(cuint destCol, cta destC, cuint srceCol, cta srceC)
		{
			if( !ui_len || (ui_ht <= destCol) || (ui_ht <= srceCol)) throw OptiVec_SizeErr();
			MCFCols_lincomb(vx_m0, ui_ht, ui_len, destCol, destC, srceCol, srceC);
		}

/////////////////////////////////////////////////////////////////////////////////////////
// Matrix transposition and rotation
//
		void transpose(cmta MA)
		{
			enforceSize(MA.ui_len, MA.ui_ht);
			MCFtranspose(vx_m0, MA.vx_m0, ui_ht, ui_len);
		}

		void hermconj(cmta MA)
		{
			enforceSize(MA.ui_len, MA.ui_ht);
			MCFhermconj(vx_m0, MA.vx_m0, ui_ht, ui_len);
		}

		void rotate90(cmta MA)
		{
			enforceSize(MA.ui_len, MA.ui_ht);
			MCFrotate90(vx_m0, MA.vx_m0, ui_ht, ui_len);
		}

		void rotate180(cmta MA)
		{
			enforceSize(MA.ui_len, MA.ui_ht);
			MCFrotate180(vx_m0, MA.vx_m0, ui_ht, ui_len);
		}

		void rotate270(cmta MA)
		{
			enforceSize(MA.ui_len, MA.ui_ht);
			MCFrotate270(vx_m0, MA.vx_m0, ui_ht, ui_len);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Matrix arithmetics
//(The following functions are more efficient than the arithmetic operators defined above)
//
		void addM(cmta MA, cmta MB)
		{
			enforceSize(MA.ui_ht, MA.ui_len);
			enforceSize(MB.ui_ht, MB.ui_len);
			MCFaddM(vx_m0, MA.vx_m0, MB.vx_m0, ui_ht, ui_len);
		}

		void addMT(cmta MA, cmta MB)
		{
			enforceSize(MA.ui_ht,  MA.ui_len);
			enforceSize(MB.ui_len, MB.ui_ht);
			MCFaddMT(vx_m0, MA.vx_m0, MB.vx_m0, ui_ht, ui_len);
		}

		void subM(cmta MA, cmta MB)
		{
			enforceSize(MA.ui_ht, MA.ui_len);
			enforceSize(MB.ui_ht, MB.ui_len);
			MCFsubM(vx_m0, MA.vx_m0, MB.vx_m0, ui_ht, ui_len);
		}

		void subMT(cmta MA, cmta MB)
		{
			enforceSize(MA.ui_ht,  MA.ui_len);
			enforceSize(MB.ui_len, MB.ui_ht);
			MCFsubMT(vx_m0, MA.vx_m0, MB.vx_m0, ui_ht, ui_len);
		}

		void subrM(cmta MA, cmta MB)
		{
			enforceSize(MA.ui_ht, MA.ui_len);
			enforceSize(MB.ui_ht, MB.ui_len);
			MCFsubM(vx_m0, MB.vx_m0, MA.vx_m0, ui_ht, ui_len);
		}

		void subrMT(cmta MA, cmta MB)
		{
			enforceSize(MA.ui_ht,  MA.ui_len);
			enforceSize(MB.ui_len, MB.ui_ht);
			MCFsubrMT(vx_m0, MA.vx_m0, MB.vx_m0, ui_ht, ui_len);
		}

		void mulC(cmta MA, cta C)
		{
			enforceSize(MA.ui_ht, MA.ui_len);
			MCFmulC(vx_m0, MA.vx_m0, ui_ht, ui_len, C);
		}

		void divC(cmta MA, cta C)
		{
			enforceSize(MA.ui_ht, MA.ui_len);
			MCFdivC(vx_m0, MA.vx_m0, ui_ht, ui_len, C);
		}

		void lincomb(cmta MA, cmta MB, cta CA, cta CB)
		{
			enforceSize(MA.ui_ht, MA.ui_len);
			enforceSize(MB.ui_ht, MB.ui_len);
			MCFlincomb(vx_m0, MA.vx_m0, MB.vx_m0, ui_ht, ui_len, CA, CB);
		}

		//mixed multiplications matrix * vector belong to fVecObj
		void mulM( cmta MA, cmta MB )
		{
			if( ui_ht != MA.ui_ht || ui_len != MB.ui_len || MB.ui_ht != MA.ui_len )throw OptiVec_SizeErr();
	 		MCFmulM( vx_m0, MA.vx_m0, MB.vx_m0, MA.ui_ht, MA.ui_len, MB.ui_len);
	 	}

		void mulMT( cmta MA, cmta MB )
		{
	 		if( ui_ht != MA.ui_ht || ui_len != MB.ui_ht || MB.ui_len != MA.ui_len )throw OptiVec_SizeErr();
		 	MCFmulMT( vx_m0, MA.vx_m0, MB.vx_m0, MA.ui_ht, MA.ui_len, MB.ui_ht);
		}

		void mulMH( cmta MA, cmta MB )
		{
	 		if( ui_ht != MA.ui_ht || ui_len != MB.ui_ht || MB.ui_len != MA.ui_len )throw OptiVec_SizeErr();
	 		MCFmulMH( vx_m0, MA.vx_m0, MB.vx_m0, MA.ui_ht, MA.ui_len, MB.ui_ht);
		}

		void TmulM( cmta MA, cmta MB )
		{
	 		if( ui_ht != MA.ui_len || ui_len != MB.ui_len || MB.ui_ht != MA.ui_ht )throw OptiVec_SizeErr();
	 		MCFTmulM( vx_m0, MA.vx_m0, MB.vx_m0, MA.ui_ht, MA.ui_len, MB.ui_len);
		}

		void HmulM( cmta MA, cmta MB )
		{
	 		if( ui_ht != MA.ui_len || ui_len != MB.ui_len || MB.ui_ht != MA.ui_ht )throw OptiVec_SizeErr();
	 		MCFHmulM( vx_m0, MA.vx_m0, MB.vx_m0, MA.ui_ht, MA.ui_len, MB.ui_len);
		}

		void TmulMT( cmta MA, cmta MB )
		{
			if( ui_ht != MA.ui_len || ui_len != MB.ui_ht || MB.ui_len != MA.ui_ht )throw OptiVec_SizeErr();
	 		MCFTmulMT( vx_m0, MA.vx_m0, MB.vx_m0, MA.ui_ht, MA.ui_len, MB.ui_ht);
		}

		void TmulMH( cmta MA, cmta MB )
		{
			if( ui_ht != MA.ui_len || ui_len != MB.ui_ht || MB.ui_len != MA.ui_ht )throw OptiVec_SizeErr();
	 		MCFTmulMH( vx_m0, MA.vx_m0, MB.vx_m0, MA.ui_ht, MA.ui_len, MB.ui_ht);
		}

		void dia_mulM( cvta MADia, cmta MB )
		{
	 		if( ui_ht != MADia.ui_size || ui_len != MB.ui_len || MB.ui_ht != MADia.ui_size )throw OptiVec_SizeErr();
	 		MCFdiamulM( vx_m0, MADia.vx_vect, MB.vx_m0, MB.ui_ht, MB.ui_len );
		}

		void dia_mulMT( cvta MADia, cmta MB )
		{
			if( ui_ht != MADia.ui_size || ui_len != MB.ui_ht || MB.ui_len != MADia.ui_size )throw OptiVec_SizeErr();
			MCFdiamulMT( vx_m0, MADia.vx_vect, MB.vx_m0, MB.ui_ht, MB.ui_len );
		}

		void mulMdia( cmta MA, cvta MBDia )
		{
			if( ui_ht != MA.ui_ht || ui_len != MBDia.ui_size || MBDia.ui_size != MA.ui_len )throw OptiVec_SizeErr();
			MCFmulMdia( vx_m0, MA.vx_m0, MBDia.vx_vect, MA.ui_ht, MA.ui_len );
		}

		void TmulMdia( cmta MA, cvta MBDia )
		{
	 		if( ui_ht != MA.ui_len || ui_len != MBDia.ui_size || MBDia.ui_size != MA.ui_ht )throw OptiVec_SizeErr();
			MCFTmulMdia( vx_m0, MA.vx_m0, MBDia.vx_vect, MA.ui_ht, MA.ui_len );
		}

/////////////////////////////////////////////////////////////////////////////////////////
//Linear Algebra
//
		int LUdecompose( uiVecObj Ind, cmta MA )
		{
			if( ui_ht != MA.ui_ht || ui_len != MA.ui_len || ui_ht != ui_len || ui_ht != Ind.ui_size )
				throw OptiVec_SizeErr();
	 		return MCFLUdecompose( vx_m0, Ind.vx_vect, MA.vx_m0, ui_len );
		}

		int LUdecompose( uiVecObj* Ind, cmta MA )
		{
	 		if( ui_ht != MA.ui_ht || ui_len != MA.ui_len || ui_ht != ui_len || ui_ht != Ind->ui_size )
	 	 		throw OptiVec_SizeErr();
			return MCFLUdecompose( vx_m0, Ind->vx_vect, MA.vx_m0, ui_len );
		 }

		int  LUDresult() const
		{
			return MCF_LUDresult();
		}

		void LUDsetEdit(const tbase& Thresh) const
		{
			MCF_LUDsetEdit(Thresh);
		}

		tbase LUDgetEdit() const
		{
			return MCF_LUDgetEdit();
		}

		void LUinv( cmta MLU, const uiVecObj& Ind )
		{
			if( ui_ht != MLU.ui_ht || ui_len != MLU.ui_len || ui_ht != ui_len || ui_ht != Ind.ui_size )
				throw OptiVec_SizeErr();
			MCFLUinv( vx_m0, MLU.vx_m0, Ind.vx_vect, ui_len );
		}

		t LUdet( int permut ) const
		{
			if( ui_ht != ui_len ) throw OptiVec_SizeErr();
			return MCFLUdet( vx_m0, ui_len, permut );
		}

/*  SVD not yet implemented:
		int SVdecompose( tMatObj *MV, tVecObj *W, cmta MA )
		{
	 		if( (MA.ui_ht < MA.ui_len ? (ui_ht != ui_len) : (ui_ht != MA.ui_ht))
			  || ui_len != MA.ui_len || ui_len != MV->ui_len || ui_len != MV->ui_ht
			  || ui_len != W->ui_size )   throw OptiVec_SizeErr();
			return MCFSVdecompose( vx_m0, MV->vx_m0, W->vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
		}

		int SVdecompose( tMatObj MV, tVecObj W, cmta MA )
		{
			if( (MA.ui_ht < MA.ui_len ? (ui_ht != ui_len) : (ui_ht != MA.ui_ht))
	 		  || ui_len != MA.ui_len || ui_len != MV.ui_len || ui_len != MV.ui_ht
	 		  || ui_len != W.ui_size )   throw OptiVec_SizeErr();
			return MCFSVdecompose( vx_m0, MV.vx_m0, W.vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
		}

		void SVDsetEdit(cta Thresh) const
		{
			MCF_SVDsetEdit(Thresh);
		}

		t SVDgetEdit() const
		{
			return MCF_SVDgetEdit();
		}
*/

		int inv( cmta MA )
		{
			if( ui_ht != MA.ui_ht || ui_len != MA.ui_len || ui_ht != ui_len )
				throw OptiVec_SizeErr();
			return MCFinv( vx_m0, MA.vx_m0, ui_len );
		}

		t det() const
		{
			if( ui_ht != ui_len ) throw OptiVec_SizeErr();
			return MCFdet( vx_m0, ui_len );
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Fourier Transforms, Convolutions, Filtering
//

		void FFT(cmta X, int dir=1)
		{
			enforcePow2();
			MCFFFT(vx_m0,X.vx_m0,ui_ht,ui_len,dir);
		}

		void Rows_FFT(cmta X, int dir=1)
		{
			enforcePow2();
			MCFRows_FFT(vx_m0,X.vx_m0,ui_ht,ui_len,dir);
		}

		void Cols_FFT(cmta X, int dir=1)
		{
			enforcePow2();
			MCFCols_FFT(vx_m0,X.vx_m0,ui_ht,ui_len,dir);
		}

		void FFTtoC(const matrix<tbase>& X)
		{
			enforcePow2();
			MFFFTtoC(vx_m0,X.vx_m0,ui_ht,ui_len);
		}

		void Rows_FFTtoC(const matrix<tbase>& X)
		{
			enforcePow2();
			MFpRows_FFTtoC(vx_m0,X.vx_m0,ui_ht,ui_len);
		}

		void Cols_FFTtoC(const matrix<tbase>& X)
		{
			enforcePow2();
			MFpCols_FFTtoC(vx_m0,X.vx_m0,ui_ht,ui_len);
		}

		void filter(cmta X, cmta Flt)
		{
			enforcePow2();
			MCFfilter(vx_m0, X.vx_m0, Flt.vx_m0, ui_ht, ui_len);
		}

		void p_FFT(cmta X, int dir=1)
		{
			enforcePow2();
			MCFpFFT(vx_m0,X.vx_m0,ui_ht,ui_len,dir);
		}

		void p_Rows_FFT(cmta X, int dir=1)
		{
			enforcePow2();
			MCFpRows_FFT(vx_m0,X.vx_m0,ui_ht,ui_len,dir);
		}

		void p_Cols_FFT(cmta X, int dir=1)
		{
			enforcePow2();
			MCFpCols_FFT(vx_m0,X.vx_m0,ui_ht,ui_len,dir);
		}

		void b_FFT(cmta X, int dir, vta Buf)
		{
			enforcePow2();
			if( Buf.ui_size < ui_size )	throw OptiVec_SizeErr();
			MCFbFFT(vx_m0,X.vx_m0,ui_ht,ui_len,dir,Buf.vx_vect);
		}

		void b_Rows_FFT(cmta X, int dir, vta Buf)
		{
			enforcePow2();
			if( Buf.ui_size < ui_size )	throw OptiVec_SizeErr();
			MCFbRows_FFT(vx_m0,X.vx_m0,ui_ht,ui_len,dir,Buf.vx_vect);
		}

		void b_Cols_FFT(cmta X, int dir, vta Buf)
		{
			enforcePow2();
			if( Buf.ui_size < ui_size )	throw OptiVec_SizeErr();
			MCFbCols_FFT(vx_m0,X.vx_m0,ui_ht,ui_len,dir,Buf.vx_vect);
		}

		void p_FFTtoC(const matrix<tbase>& X)
		{
			enforcePow2();
			MFpFFTtoC(vx_m0,X.vx_m0,ui_ht,ui_len);
		}

		void p_Rows_FFTtoC(const matrix<tbase>& X)
		{
			enforcePow2();
			MFpRows_FFTtoC(vx_m0,X.vx_m0,ui_ht,ui_len);
		}

		void p_Cols_FFTtoC(const matrix<tbase>& X)
		{
			enforcePow2();
			MFpCols_FFTtoC(vx_m0,X.vx_m0,ui_ht,ui_len);
		}

		void b_FFTtoC(const matrix<tbase>& X, vta Buf )
		{
			enforcePow2();
			if( Buf.ui_size < ui_size )	throw OptiVec_SizeErr();
			MFbFFTtoC(vx_m0,X.vx_m0,ui_ht,ui_len,Buf.vx_vect);
		}

		void b_Rows_FFTtoC(const matrix<tbase>& X, vta Buf )
		{
			enforcePow2();
			if( Buf.ui_size < ui_size )	throw OptiVec_SizeErr();
			MFbRows_FFTtoC(vx_m0,X.vx_m0,ui_ht,ui_len,Buf.vx_vect);
		}

		void b_Cols_FFTtoC(const matrix<tbase>& X, vta Buf )
		{
			enforcePow2();
			if( Buf.ui_size < ui_size )	throw OptiVec_SizeErr();
			MFbCols_FFTtoC(vx_m0,X.vx_m0,ui_ht,ui_len,Buf.vx_vect);
		}

		void p_filter(cmta X, cmta Flt)
		{
			enforcePow2();
			MCFpfilter(vx_m0, X.vx_m0, Flt.vx_m0, ui_ht, ui_len);
		}

		void l_FFT(cmta X, int dir=1)
		{
			enforcePow2();
			MCFlFFT(vx_m0,X.vx_m0,ui_ht,ui_len,dir);
		}

		void l_Rows_FFT(cmta X, int dir=1)
		{
			enforcePow2();
			MCFlRows_FFT(vx_m0,X.vx_m0,ui_ht,ui_len,dir);
		}

		void l_Cols_FFT(cmta X, int dir=1)
		{
			enforcePow2();
			MCFlCols_FFT(vx_m0,X.vx_m0,ui_ht,ui_len,dir);
		}

		void l_FFTtoC(const matrix<tbase>& X)
		{
			enforcePow2();
			MFlFFTtoC(vx_m0,X.vx_m0,ui_ht,ui_len);
		}

		void l_filter(cmta X, cmta Flt)
		{
			enforcePow2();
			MCFlfilter(vx_m0, X.vx_m0, Flt.vx_m0, ui_ht, ui_len);
		}

		void s_FFT(cmta X, int dir=1)
		{
			enforcePow2();
			MCFsFFT(vx_m0,X.vx_m0,ui_ht,ui_len,dir);
		}

		void s_Rows_FFT(cmta X, int dir=1)
		{
			enforcePow2();
			MCFsRows_FFT(vx_m0,X.vx_m0,ui_ht,ui_len,dir);
		}

		void s_Cols_FFT(cmta X, int dir=1)
		{
			enforcePow2();
			MCFsCols_FFT(vx_m0,X.vx_m0,ui_ht,ui_len,dir);
		}

		void s_FFTtoC(const matrix<tbase>& X)
		{
			enforcePow2();
			MFsFFTtoC(vx_m0,X.vx_m0,ui_ht,ui_len);
		}

		void s_filter(cmta X, cmta Flt)
		{
			enforcePow2();
			MCFsfilter(vx_m0, X.vx_m0, Flt.vx_m0, ui_ht, ui_len);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Input and Output
//
		void fprint(FILE* stream, unsigned linewidth) const
		{
			enforceSize();
			MCFfprint(stream,vx_m0,ui_ht, ui_len,linewidth);
		}

		void cprint() const
		{
			enforceSize();
			MCFcprint(vx_m0,ui_ht,ui_len);
		}

		void print() const
		{
			fprint(stdout,80);
		}

		void write(FILE* stream) const
		{
			enforceSize();
			MCFwrite(stream,vx_m0,ui_ht,ui_len);
		}

		void read(FILE* stream)
		{
			enforceSize();
			MCFread(vx_m0,ui_ht,ui_len,stream);
		}

		void setWriteFormat(char* formatString) const
		{
			VCF_setWriteFormat(formatString);
		}

		void setWriteSeparate(char* sepString) const
		{
			VCF_setWriteSeparate(sepString);
		}

		void store(FILE* stream) const
		{
			enforceSize();
			fwrite(&ui_ht,sizeof(ui),1,stream);
			fwrite(&ui_len,sizeof(ui),1,stream);
			VCF_store(stream,vx_m0,ui_size);
		}

		void recall(FILE* stream)
		{
			fread(&ui_ht,sizeof(ui),1,stream);
			fread(&ui_len,sizeof(ui),1,stream);
			setSize(ui_ht,ui_len);
			VCF_recall(vx_m0,ui_size,stream);
		}
	};


	inline vt tVecObj::operator*(cmta MY)
	{
		tVecObj tmp(this->ui_size);
		tmp.enforceSize();
		VCFmulM(tmp.vx_vect,vx_vect,MY.vx_m0,MY.ui_ht,MY.ui_len);
		return tmp;
	}

	inline void tVecObj::Row_extract(cmta MA, const ui iRow)
	{
		if( MA.ui_ht <= iRow || ui_size < MA.ui_len || !ui_size ) throw OptiVec_SizeErr();
		MCFRow_extract( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len, iRow );
	}

	inline void tVecObj::Col_extract(cmta MA, const ui iCol)
	{
		if( MA.ui_len <= iCol || ui_size < MA.ui_ht || !ui_size ) throw OptiVec_SizeErr();
		MCFCol_extract( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len, iCol );
	}

	inline void tVecObj::Dia_extract(cmta MA)
	{
		if( !ui_size || ui_size != MA.ui_len || (MA.ui_len != MA.ui_ht) )throw OptiVec_SizeErr();
		MCFDia_extract( vx_vect, MA.vx_m0, MA.ui_len );
	}

	inline void tVecObj::Rows_maxReIm(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_ht ) throw OptiVec_SizeErr();
		MCFRows_maxReIm( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::Cols_maxReIm(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_len ) throw OptiVec_SizeErr();
		MCFCols_maxReIm( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::Rows_minReIm(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_ht ) throw OptiVec_SizeErr();
		MCFRows_minReIm( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::Cols_minReIm(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_len ) throw OptiVec_SizeErr();
		MCFCols_minReIm( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::Rows_absmaxReIm(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_ht ) throw OptiVec_SizeErr();
		MCFRows_absmaxReIm( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::Cols_absmaxReIm(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_len ) throw OptiVec_SizeErr();
		MCFCols_absmaxReIm( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::Rows_absminReIm(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_ht ) throw OptiVec_SizeErr();
		MCFRows_absminReIm( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::Cols_absminReIm(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_len ) throw OptiVec_SizeErr();
		MCFCols_absminReIm( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::Rows_sabsmax(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_ht ) throw OptiVec_SizeErr();
		MCFRows_sabsmax( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::Cols_sabsmax(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_len ) throw OptiVec_SizeErr();
		MCFCols_sabsmax( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::Rows_sabsmin(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_ht ) throw OptiVec_SizeErr();
		MCFRows_sabsmin( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::Cols_sabsmin(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_len ) throw OptiVec_SizeErr();
		MCFCols_sabsmin( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::Rows_cabsmax(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_ht ) throw OptiVec_SizeErr();
		MCFRows_cabsmax( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::Cols_cabsmax(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_len ) throw OptiVec_SizeErr();
		MCFCols_cabsmax( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::Rows_cabsmin(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_ht ) throw OptiVec_SizeErr();
		MCFRows_cabsmin( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::Cols_cabsmin(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_len ) throw OptiVec_SizeErr();
		MCFCols_cabsmin( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void vector<tbase>::Rows_absmax(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_ht ) throw OptiVec_SizeErr();
		MCFRows_absmax( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void vector<tbase>::Cols_absmax(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_len ) throw OptiVec_SizeErr();
		MCFCols_absmax( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void vector<tbase>::Rows_absmin(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_ht ) throw OptiVec_SizeErr();
		MCFRows_absmin( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void vector<tbase>::Cols_absmin(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_len ) throw OptiVec_SizeErr();
		MCFCols_absmin( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::Rows_sum(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_ht ) throw OptiVec_SizeErr();
		MCFRows_sum( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::Cols_sum(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_len ) throw OptiVec_SizeErr();
		MCFCols_sum( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::Rows_prod(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_ht ) throw OptiVec_SizeErr();
		MCFRows_prod( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::Cols_prod(cmta MA)
	{
		if( !MA.ui_ht || !MA.ui_len || ui_size < MA.ui_len ) throw OptiVec_SizeErr();
		MCFCols_prod( vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::MmulV( cmta MA, cvta X )
	{
		if( ui_size != MA.ui_ht || X.ui_size != MA.ui_len ) throw OptiVec_SizeErr();
		MCFmulV( vx_vect, MA.vx_m0, X.vx_vect, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::MTmulV( cmta MA, cvta X )
	{
		if( ui_size != MA.ui_len || X.ui_size != MA.ui_ht ) throw OptiVec_SizeErr();
		MCFTmulV( vx_vect, MA.vx_m0, X.vx_vect, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::mulM( cvta X, cmta MA )
	{
		if( ui_size != MA.ui_len || X.ui_size != MA.ui_ht ) throw OptiVec_SizeErr();
		VCFmulM( vx_vect, X.vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::mulMT( cvta X, cmta MA )
	{
		if( ui_size != MA.ui_ht || X.ui_size != MA.ui_len ) throw OptiVec_SizeErr();
		VCFmulMT( vx_vect, X.vx_vect, MA.vx_m0, MA.ui_ht, MA.ui_len );
	}

	inline void tVecObj::LUsolve( cmta MLU, cvta B, const uiVecObj& Ind )
	{
		if( ui_size != MLU.ui_len || ui_size != MLU.ui_ht ||
		     ui_size != B.ui_size || ui_size != Ind.ui_size ) throw OptiVec_SizeErr();
		MCFLUsolve( vx_vect, MLU.vx_m0, B.vx_vect, Ind.vx_vect, MLU.ui_len );
	}

	inline void tVecObj::LUimprove( cvta B, cmta MA, cmta MLU,
	                                                              const uiVecObj& Ind )
	{
		if( ui_size != MLU.ui_len || ui_size != MLU.ui_ht ||
		    ui_size != MA.ui_len || ui_size != MA.ui_ht ||
		    ui_size != B.ui_size || ui_size != Ind.ui_size ) throw OptiVec_SizeErr();
		MCFLUimprove( vx_vect, B.vx_vect, MA.vx_m0, MLU.vx_m0, Ind.vx_vect, MLU.ui_len );
	}

/* SVD not yet implemented:
	inline void tVecObj::SVsolve( cmta MU, cmta MV, cvta W, cvta B )
	{ // result may overwrite B only for "normal", not for over- or under-determined systems
		if( MU.ui_ht < MU.ui_len || MU.ui_len != MV.ui_len || MV.ui_len != MV.ui_ht ||
		    B.ui_size != ui_size || W.ui_size != MV.ui_len || ui_size != MU.ui_ht )
		    throw OptiVec_SizeErr();
		MCFSVsolve( vx_vect, MU.vx_m0, MV.vx_m0, W.vx_vect, B.vx_vect, B.ui_size, MU.ui_len );
	}
*/
	inline int tVecObj::solve( cmta MA, cvta B )
	{
		if( ui_size != MA.ui_len || ui_size != MA.ui_ht ||
		    ui_size != B.ui_size ) throw OptiVec_SizeErr();
		return MCFsolve( vx_vect, MA.vx_m0, B.vx_vect, MA.ui_len );
	}

/*	inline int tVecObj::solveBySVD( cmta MA, cvta B )
	{
		if(ui_size != MA.ui_len || (B.ui_size != MA.ui_ht && B.vx_vect != vx_vect)) throw OptiVec_SizeErr();
		return MCFsolveBySVD( vx_vect, MA.vx_m0, B.vx_vect, MA.ui_ht, MA.ui_len );
	}

	inline int tVecObj::safeSolve( cmta MA, cvta B )
	{
		if( ui_size != MA.ui_len || ui_size != MA.ui_ht ||
		    ui_size != B.ui_size ) throw OptiVec_SizeErr();
		return MCFsafeSolve( vx_vect, MA.vx_m0, B.vx_vect, MA.ui_len );
	}
*/

#ifdef CDMATOBJ_H
	inline void cfMatObj::CDtoCF(const cdMatObj& X)
	{
		enforceSize(X.ui_ht, X.ui_len);
		V_CDtoCF(vx_m0,X.vx_m0,ui_size);
	}

	inline void cdMatObj::CFtoCD(const cfMatObj& X)
	{
		enforceSize(X.ui_ht, X.ui_len);
		V_CFtoCD(vx_m0,X.vx_m0,ui_size);
	}
#endif //CDMATOBJ_H

#ifdef CEMATOBJ_H
#if (defined __BORLANDC__) && !(defined _WIN64) //80-bit numbers defined
	inline void cfMatObj::CEtoCF(const ceMatObj& X)
	{
		enforceSize(X.ui_ht, X.ui_len);
		V_CEtoCF(vx_m0,X.vx_m0,ui_size);
	}

	inline void ceMatObj::CFtoCE(const cfMatObj& X)
	{
		enforceSize(X.ui_ht, X.ui_len);
		V_CFtoCE(vx_m0,X.vx_m0,ui_size);
	}
#endif //__BORLANDC__
#endif //CEMATOBJ_H

#if defined _MSC_VER || (defined __BORLANDC__ && __BORLANDC__ >= 0x500)
}  // end of namespace OptiVec
#endif

#undef cvta
#undef cmta
#undef cua
#undef cuint
#undef cta

#undef vta
#undef mta
#undef ta

#undef vt
#undef mt
#undef t
#undef tVector
#undef tMatrix
#undef tVecObj
#undef tMatObj

#undef vtbase
#undef tbase

#undef tVecObj
#undef tMatObj
#ifdef __BORLANDC__
#pragma warn .inl
#endif

#define CFMATOBJ_H
#endif //CFMATOBJ_H
