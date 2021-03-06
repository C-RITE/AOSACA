/* pfVecObj.H

  object-oriented interface for OptiVec vector functions of type "fPolar"

  This file was written by Brian Dale, Case Western Reserve Univ.

  Copyright (c) 1996-2017 by OptiCode
  All Rights Reserved.
*/

#ifndef PFVECOBJ_H

#include <VecObj.h>
#include <uiVecObj.h>
#include <fVecObj.h>

#define cvta const vector<fPolar>&
#define cuia const ui&
#define cta  const fPolar&

#define vta  vector<fPolar>&
#define uia  ui&
#define ta   fPolar&

#define vt   vector<fPolar>
#define t    fPolar
#define tVector pfVector

#define vtbase vector<float>
#define tbase  float

#define tVecObj pfVecObj
#ifdef __BORLANDC__
#pragma warn -inl
#endif

#if defined _MSC_VER || (defined __BORLANDC__ && __BORLANDC__ >= 0x500)
namespace OptiVec{
#endif

	VECOBJ_NEW_TEMPLATE class vector<t>
	{
#ifdef _MSC_VER 
     public:
		tVector vx_vect;
		ui      ui_size;
#else
		tVector vx_vect;
		ui      ui_size;
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
		friend class vector<fPolar>;
		friend class vector<dPolar>;
		friend class vector<ePolar>;
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
		friend class matrix<float>;
		friend class matrix<double>;
		friend class matrix<extended>;
		friend class matrix<fComplex>;
		friend class matrix<dComplex>;
		friend class matrix<eComplex>;
#endif
	public:
////////////////////////////////////////////////////////////////////////////////////////////////////
//constructors and destructors
//
		vector(){	vx_vect=0;	ui_size=0;}

		vector(cuia size)
		{	if(size){	vx_vect= VPF_vector(size);	ui_size= size;}
			else{	vx_vect= 0;	ui_size= 0;}}

		vector(cuia size, cta fill)
		{	if(!size) //size == 0
			{	vx_vect=0;	ui_size=0;}
			else if(!(fill.Mag) && !(fill.Arg)) //fill == 0.0
			{	vx_vect= VPF_vector0(size);	ui_size= size;}
			else
			{	vx_vect= VPF_vector(size);	ui_size= size;	VPF_equC(vx_vect,ui_size,fill);}}

		vector(cvta init)
		{	if(init.ui_size)
			{ vx_vect= VPF_vector(init.ui_size);	ui_size= init.ui_size;	VPF_equV(vx_vect, init.vx_vect, ui_size);}
			else
			{	vx_vect=0;	ui_size=0;}}

		~vector(){	if(vx_vect) V_free(vx_vect);}


////////////////////////////////////////////////////////////////////////////////////////////////////
//management
//
		void enforceSize()const{ if(!ui_size) throw OptiVec_EmptySizeErr();}

		void enforceSize(ui size)const{ if(ui_size != size) throw OptiVec_SizeErr();}

		void enforcePow2()const{ if(!ui_isipow2(ui_size)) throw OptiVec_Pow2SizeErr();}

		void setSize(cuia size)
		{	if(!size) dealloc(); //can set size to zero
			if(ui_size != size) //only allocate if different sizes
			{	if(vx_vect) V_free(vx_vect);
				vx_vect= VPF_vector(size);
				ui_size= size;}}

		void setSize(cvta X)
		{	if(!X.ui_size) dealloc(); //can set size to zero
			if(ui_size != X.ui_size) //only allocate if different sizes
			{	if(vx_vect) V_free(vx_vect);
				vx_vect= VPF_vector(X.ui_size);
				ui_size= X.ui_size;}}

		ui       getSize() const { return ui_size; }
		tVector  getVector() const { return vx_vect; }

#if !defined (_CRTDBG_MAP_ALLOC) // crtdbg.h defines free as a macro
		void free()
		{	if(vx_vect) V_free(vx_vect);
			vx_vect=0;
			ui_size=0;}
#endif

		void dealloc()
		{	if(vx_vect) V_free(vx_vect);
			vx_vect=0;
			ui_size=0;}

		ta at(ui n)const
		{	enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return vx_vect[n];}

		
///////////////////////////////////////////////////////////////////////////////////////////////////
// Overloaded operators
//
		vta operator=(cvta vect)
		{	setSize(vect.ui_size);
			enforceSize();
			VPF_equV(vx_vect, vect.vx_vect, ui_size);
			return *this;}

		vta operator=(cta scalar)
		{	enforceSize();
			VPF_equC(vx_vect, ui_size, scalar);
			return *this;}

		//this is the only really dangerous operator
		ta operator[](const int& index){return vx_vect[index];}

		vta operator*=(cvta X)
		{	enforceSize();
			VPF_mulV(vx_vect, vx_vect, X.vx_vect, ui_size);
			return *this;}

		vta operator*=(cta scalar)
		{	enforceSize();
			VPF_mulC(vx_vect,vx_vect,ui_size,scalar);
			return *this;}

		vt operator*(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VPF_mulV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator*(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VPF_mulC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator*(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VPF_mulC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}

		vt operator/(cvta rhs)
		{	tVecObj tmp((this->ui_size<rhs.ui_size) ? this->ui_size : rhs.ui_size);
			tmp.enforceSize();
			VPF_divV(tmp.vx_vect,this->vx_vect,rhs.vx_vect,tmp.ui_size);
			return tmp;}

		vt operator/(cta C)
		{	tVecObj tmp(this->ui_size);
			tmp.enforceSize();
			VPF_divC(tmp.vx_vect,this->vx_vect,tmp.ui_size,C);
			return tmp;}

		friend vt operator/(cta C, cvta X)
		{	tVecObj tmp(X.ui_size);
			tmp.enforceSize();
			VPF_divrC(tmp.vx_vect,X.vx_vect,tmp.ui_size,C);
			return tmp;}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//OptiVec functions in member format
//

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//from VXstd.h
//

/////////////////////////////////////////////////////////////////////////////////////////
//Generation
//
		//these are not recommended, better to use constructors
		void _vector(ui size) //underscore necessary to avoid confusing with constructor
		{
			dealloc();
			if(size) vx_vect= VPF_vector(size);
			ui_size=size;
		}

		void vector0(ui size)
		{
			dealloc();
			if(size) vx_vect= VPF_vector0(size);
			ui_size=size;
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Addressing single vector elements
//
		t* Pelement(ui n)const
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VPF_Pelement(vx_vect,n);
		}

		t element(ui n)const
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VPF_element(vx_vect,n);
		}

		void getElement(t *y, ui n)
		{
			enforceSize();
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			return VPF_getElement(y,vx_vect,n);
		}

		void setElement(ui n, cta C)
		{
			if(ui_size<=n) throw OptiVec_SmallSizeErr();
			VPF_setElement(vx_vect,n, C);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Initialization
//
		void equ0()
		{
			enforceSize();
			VPF_equ0(vx_vect,ui_size);
		}

		void equ1()
		{
			enforceSize();
			VPF_equ1(vx_vect,ui_size);
		}

		void equm1()
		{
			enforceSize();
			VPF_equm1(vx_vect, ui_size);
		}

		void equC(cta C)
		{
			enforceSize();
			VPF_equC(vx_vect, ui_size, C);
		}

		void equV(cvta X)
		{
			enforceSize();
			VPF_equV(vx_vect, X.vx_vect, ui_size);
		}

		void principal(cvta X)
		{
			enforceSize();
			VPF_principal(vx_vect, X.vx_vect, ui_size);
		}

		//PtoMagArg member of fVecObj

		void MagArgtoP(const vtbase& Mag, const vtbase& Arg)
		{
			enforceSize();
			VF_MagArgtoP(vx_vect, Mag.vx_vect, Arg.vx_vect, ui_size);
		}

		void MagArgtoPrincipal(const vtbase& Mag, const vtbase& Arg)
		{
			enforceSize();
			VF_MagArgtoPrincipal(vx_vect, Mag.vx_vect, Arg.vx_vect, ui_size);
		}

		//PtoMag member of fVecObj

		void MagtoP(const vtbase& Mag)
		{
			enforceSize();
			VF_MagtoP(vx_vect, Mag.vx_vect, ui_size);
		}

		//PtoArg member of fVecObj

		void ArgtoP(const vtbase& Arg)
		{
			enforceSize();
			VF_ArgtoP(vx_vect, Arg.vx_vect, ui_size);
		}

		//defined by including cfVecObj.h
		void CtoP(const cfVecObj&);

		//PtoC member of cfVecObj

		//PtoNorm member of fVecObj
		//PtoReIm member of fVecObj
		
		void ReImtoP(const vtbase& Re, const vtbase& Im)
		{
			enforceSize();
			VF_ReImtoP(vx_vect, Re.vx_vect, Im.vx_vect, ui_size);
		}

		//PtoRe member of fVecObj
		//PtoIm member of fVecObj
		//abs member of fVecObj


/////////////////////////////////////////////////////////////////////////////////////////
//Data-type interconversions
//
		//defined by including cdVecObj.h
		void PDtoPF(const pdVecObj& X);

		//defined by including ceVecObj.h
		void PEtoPF(const peVecObj& X);



/////////////////////////////////////////////////////////////////////////////////////////
//Index-oriented manipulations
//
		void reflect()
		{
			enforceSize();
			VPF_reflect(vx_vect, ui_size);
		}

		void _delete(ui pos)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VPF_delete(vx_vect, ui_size, pos);
		}

		void insert(ui pos, cta C)
		{
			enforceSize();
			if(ui_size<=pos) throw OptiVec_SmallSizeErr();
			VPF_insert(vx_vect, ui_size, pos, C);
		}

		void rev(cvta X)
		{
			enforceSize();
			VPF_rev(vx_vect, X.vx_vect, ui_size);
		}

		void rotate( cvta X, ssize_t pos )
		{
			enforceSize();
			VPF_rotate(vx_vect, X.vx_vect, ui_size, pos);
		}

		void rotate_buf( cvta X, ssize_t pos, cvta Buf )
		{
			enforceSize();
			VPF_rotate_buf(vx_vect, X.vx_vect, ui_size, pos, Buf.vx_vect);
		}

		void indpick(const uiVecObj& Ind, cvta X)
		{
			enforceSize();
			VPF_indpick(vx_vect, Ind.vx_vect, ui_size, X.vx_vect);
		}

		void indput(cvta X, const uiVecObj& Ind)
		{
			enforceSize();
			X.enforceSize();
			VPF_indput(vx_vect,X.vx_vect,Ind.vx_vect,X.ui_size);
		}

		void subvector(cvta X, ssize_t step, ui start=0)
		{
			enforceSize();
			if(vx_vect==X.vx_vect) throw OptiVec_Err();
			VPF_subvector(vx_vect, ui_size, X.vx_vect+start, step);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Functions of a sub-set of elements
//
		void subvector_equC(const ui& samp, cta C)
		{
			enforceSize();
			VPF_subvector_equC(vx_vect, ((ui_size-1)/samp)+1, samp, C);
		}

		void subvector_equV(const ui& samp, cvta X)
		{
			enforceSize();
			VPF_subvector_equV(vx_vect, ((ui_size-1)/samp)+1, samp, X.vx_vect);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//One-Dimensional Vector Operations
//

		tbase absmax() const
		{
			enforceSize();
			return VPF_absmax(vx_vect, ui_size);
		}

		tbase absmin() const
		{
			enforceSize();
			return VPF_absmin(vx_vect, ui_size);
		}

		tbase absmaxind(ui* Ind) const
		{
			enforceSize();
			return VPF_absmaxind(Ind, vx_vect, ui_size);
		}

		tbase absminind(ui* Ind) const
		{
			enforceSize();
			return VPF_absminind(Ind, vx_vect, ui_size);
		}

		t prod() const
		{
			enforceSize();
			return VPF_prod(vx_vect, ui_size);
		}

		void runprod(cvta X)
		{
			enforceSize();
			VPF_runprod(vx_vect, X.vx_vect, ui_size);
		}

		int iselementC(cta C) const
		{
			enforceSize();
			return VPF_iselementC(vx_vect, ui_size, C);
		}

		ui iselementV(cvta X, cvta Tab)
		{
			enforceSize();
			Tab.enforceSize();
			return VPF_iselementV(vx_vect, X.vx_vect,
				ui_size, Tab.vx_vect, Tab.ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Statistical Functions and Building Blocks
//


/////////////////////////////////////////////////////////////////////////////////////////
//Fourier Transforms, Convolutions, Filtering
//


/////////////////////////////////////////////////////////////////////////////////////////
//Analysis
//


/////////////////////////////////////////////////////////////////////////////////////////
//Geometrical Vector Arithmetics
//


/////////////////////////////////////////////////////////////////////////////////////////
//Input and Output
//
		void fprint(FILE* stream, const unsigned& nperline, const unsigned& linewidth) const
		{
			enforceSize();
			VPF_fprint(stream, vx_vect, ui_size, nperline, linewidth);
		}

		void  cprint( const unsigned& nperline ) const
		{
			enforceSize();
			VPF_cprint( vx_vect, ui_size, nperline);
		}

		void print(const unsigned& npl) const
		{
			enforceSize();
			VPF_print(vx_vect, ui_size, npl);
		}

		void write(FILE* stream) const
		{
			enforceSize();
			VPF_write(stream, vx_vect, ui_size);
		}

		void read(FILE* stream)
		{
			enforceSize();
			VPF_read(vx_vect, ui_size, stream);
		}


//nwrite
//nread

		void setWriteFormat(char* formatString) const
		{
			VPF_setWriteFormat(formatString);
		}

		void setWriteSeparate(char* sepString) const
		{
			VPF_setWriteSeparate(sepString);
		}

//setNWriteSeparate

		void store(FILE* stream) const
		{
			enforceSize();
			fwrite(&ui_size,sizeof(ui),1,stream);
			VPF_store(stream,vx_vect,ui_size);
		}

		void recall(FILE* stream)
		{
			ui sz;
			fread(&sz,sizeof(ui),1,stream);
			setSize(sz);
			VPF_recall(vx_vect,ui_size,stream);
		}


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
//from VXmath.h
//


/////////////////////////////////////////////////////////////////////////////////////////
//Rounding
//


/////////////////////////////////////////////////////////////////////////////////////////
//Comparisons
//

		ui cmp_eq0(cvta X)
		{
			enforceSize();
			return VPF_cmp_eq0(vx_vect,X.vx_vect,ui_size);
		}

		ui cmp_ne0(cvta X)
		{
			enforceSize();
			return VPF_cmp_ne0(vx_vect,X.vx_vect,ui_size);
		}

		ui cmp_eqC(cvta X, cta C)
		{
			enforceSize();
			return VPF_cmp_eqC(vx_vect,X.vx_vect,ui_size,C);
		}

		ui cmp_neC(cvta X, cta C)
		{
			enforceSize();
			return VPF_cmp_neC(vx_vect,X.vx_vect,ui_size,C);
		}

		ui cmp_eqV(cvta X, cvta Y)
		{
			enforceSize();
			return VPF_cmp_eqV(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
		}

		ui cmp_neV(cvta X, cvta Y)
		{
			enforceSize();
			return VPF_cmp_neV(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
		}

		ui cnt_eq0()  const
		{
			enforceSize();
			return VPF_cnt_eq0(vx_vect, ui_size);
		}

		ui cnt_ne0()  const
		{
			enforceSize();
			return VPF_cnt_ne0(vx_vect, ui_size);
		}

		ui cnt_eqC(cta C) const
		{
			enforceSize();
			return VPF_cnt_eqC(vx_vect,ui_size,C);
		}

		ui cnt_neC(cta C) const
		{
			enforceSize();
			return VPF_cnt_neC(vx_vect,ui_size,C);
		}

		ui cnt_eqV(cvta Y) const
		{
			enforceSize();
			return VPF_cnt_eqV(vx_vect,Y.vx_vect,ui_size);
		}

		ui cnt_neV(cvta Y) const
		{
			enforceSize();
			return VPF_cnt_neV(vx_vect,Y.vx_vect,ui_size);
		}

			//indexed comparisons are members of uiVecObj


/////////////////////////////////////////////////////////////////////////////////////////
//Standard Arithmetics or Basic Arithmetics
//
		void replaceNAN(cvta X, cta C)
		{
			enforceSize();
			VPF_replaceNAN( vx_vect, X.vx_vect, ui_size, C);
		}

		void mulC(cvta X, cta C)
		{
			enforceSize();
			VPF_mulC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divC(cvta X, cta C)
		{
			enforceSize();
			VPF_divC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divrC(cvta X, cta C)
		{
			enforceSize();
			VPF_divrC(vx_vect, X.vx_vect, ui_size, C);
		}

		void mulReC(cvta X, const tbase& C)
		{
			enforceSize();
			VPF_mulReC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divReC(cvta X, const tbase& C)
		{
			enforceSize();
			VPF_divReC(vx_vect, X.vx_vect, ui_size, C);
		}

		void divrReC(cvta X, const tbase& C)
		{
			enforceSize();
			VPF_divrReC(vx_vect, X.vx_vect, ui_size, C);
		}

		void mulV(cvta X, cvta Y)
		{
			enforceSize();
			VPF_mulV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divV(cvta X, cvta Y)
		{
			enforceSize();
			VPF_divV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divrV(cvta X, cvta Y)
		{
			enforceSize();
			VPF_divrV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void mulReV(cvta X, vtbase Y)
		{
			enforceSize();
			VPF_mulReV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divReV(cvta X, vtbase Y)
		{
			enforceSize();
			VPF_divReV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}

		void divrReV(cvta X, vtbase Y)
		{
			enforceSize();
			VPF_divrReV(vx_vect, X.vx_vect, Y.vx_vect, ui_size);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Accumulation
//


/////////////////////////////////////////////////////////////////////////////////////////
//Bit-wise operations
//


/////////////////////////////////////////////////////////////////////////////////////////
//Functions of a sub-set of elements
//
		//subvector_equC, subvector_equV defined above

		void subvector_mulC(const ui& samp, cta C)
		{
			enforceSize();
			VPF_subvector_mulC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_divC(const ui& samp, cta C)
		{
			enforceSize();
			VPF_subvector_divC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_divrC(const ui& samp, cta C)
		{
			enforceSize();
			VPF_subvector_divrC(vx_vect,1+(ui_size-1)/samp,samp,C);
		}

		void subvector_mulV(const ui& samp, cvta X)
		{
			enforceSize();
			VPF_subvector_mulV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void subvector_divV(const ui& samp, cvta X)
		{
			enforceSize();
			VPF_subvector_divV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}

		void subvector_divrV(const ui& samp, cvta X)
		{
			enforceSize();
			VPF_subvector_divrV(vx_vect,1+(ui_size-1)/samp,samp,X.vx_vect);
		}


/////////////////////////////////////////////////////////////////////////////////////////
//Mathematical Functions
//
		int neg(cvta X)
		{
			enforceSize();
			return VPF_neg(vx_vect, X.vx_vect, ui_size);
		}

		int conj(cvta X)
		{
			enforceSize();
			return VPF_conj(vx_vect,X.vx_vect,ui_size);
		}

		//abs defined above

		int inv(cvta X)
		{
			enforceSize();
			return VPF_inv(vx_vect, X.vx_vect, ui_size);
		}

		int square(cvta X)
		{
			enforceSize();
			return VPF_square(vx_vect,X.vx_vect,ui_size);
		}

		int cubic(cvta X)
		{
			enforceSize();
			return VPF_cubic(vx_vect,X.vx_vect,ui_size);
		}

		int quartic(cvta X)
		{
			enforceSize();
			return VPF_quartic(vx_vect,X.vx_vect,ui_size);
		}

		int ipow(cvta X, int pow)
		{
			enforceSize();
			return VPF_ipow(vx_vect, X.vx_vect, ui_size, pow);
		}

		int u_square(cvta X)
		{
			enforceSize();
			return VPFu_square(vx_vect,X.vx_vect,ui_size);
		}

		int u_cubic(cvta X)
		{
			enforceSize();
			return VPFu_cubic(vx_vect,X.vx_vect,ui_size);
		}

		int u_quartic(cvta X)
		{
			enforceSize();
			return VPFu_quartic(vx_vect,X.vx_vect,ui_size);
		}

		int u_ipow(cvta X, int pow)
		{
			enforceSize();
			return VPFu_ipow(vx_vect, X.vx_vect, ui_size, pow);
		}

		int powReExpo(cvta X, const tbase& expo)
		{
			enforceSize();
			return VPF_powReExpo(vx_vect,X.vx_vect,ui_size,expo);
		}

		int sqrt(cvta X)
		{
			enforceSize();
			return VPF_sqrt(vx_vect,X.vx_vect,ui_size);
		}

		//logtoC member of cfVecObj
		//log2toC member of cfVecObj
		//log10toC member of cfVecObj
		//lntoC member of cfVecObj

		//defined by including cfVecObj.h
		int exptoP(const cfVecObj& X);

	};


	inline ui uiVecObj::cmp_eq0ind(cvta X)
	{
		enforceSize();
		return VPF_cmp_eq0ind(vx_vect,X.vx_vect,ui_size);
	}

	inline ui uiVecObj::cmp_ne0ind(cvta X)
	{
		enforceSize();
		return VPF_cmp_ne0ind(vx_vect,X.vx_vect,ui_size);
	}

	inline ui uiVecObj::cmp_eqCind(cvta X, cta C)
	{
		enforceSize();
		return VPF_cmp_eqCind(vx_vect,X.vx_vect,ui_size,C);
	}

	inline ui uiVecObj::cmp_neCind(cvta X, cta C)
	{
		enforceSize();
		return VPF_cmp_neCind(vx_vect,X.vx_vect,ui_size,C);
	}

	inline ui uiVecObj::cmp_eqVind(cvta X, cvta Y)
	{
		enforceSize();
		return VPF_cmp_eqVind(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
	}

	inline ui uiVecObj::cmp_neVind(cvta X, cvta Y)
	{
		enforceSize();
		return VPF_cmp_neVind(vx_vect,X.vx_vect,Y.vx_vect,ui_size);
	}

#ifdef UBVECOBJ_H
#endif //UBVECOBJ_H

#ifdef USVECOBJ_H
#endif //USVECOBJ

#ifdef UVECOBJ_H
#endif //UVECOBJ_H

#ifdef ULVECOBJ_H
#endif //ULVECOBJ_H

#ifdef BIVECOBJ_H
#endif //BIVECOBJ_H

#ifdef SIVECOBJ_H
#endif //SIVECOBJ_H

#ifdef IVECOBJ_H
#endif //IVECOBJ_H

#ifdef LIVECOBJ_H
#endif //LIVECOBJ_H

#ifdef QIVECOBJ_H
#endif //QIVECOBJ_H

#ifdef FVECOBJ_H
	inline void vtbase::PtoMagArg(fVecObj* Arg, cvta X)
	{
		enforceSize();
		if(ui_size != Arg->ui_size) throw OptiVec_SizeErr();
		VF_PtoMagArg(vx_vect, Arg->vx_vect, X.vx_vect, ui_size);
	}

	inline void vtbase::PtoMagArg(fVecObj Arg, cvta X)
	{
		enforceSize();
		if(ui_size != Arg.ui_size) throw OptiVec_SizeErr();
		VF_PtoMagArg(vx_vect, Arg.vx_vect, X.vx_vect, ui_size);
	}

	inline void vtbase::PtoMag(cvta X)
	{
		enforceSize();
		VF_PtoMag(vx_vect,X.vx_vect,ui_size);
	}

	inline void vtbase::PtoArg(cvta X)
	{
		enforceSize();
		VF_PtoArg(vx_vect,X.vx_vect,ui_size);
	}

	inline void vtbase::PtoNorm(cvta cplx)
	{
		enforceSize();
		VF_PtoNorm(vx_vect, cplx.vx_vect, ui_size);
	}

	inline void vtbase::PtoReIm(fVecObj* Im, cvta X)
	{
		enforceSize();
		if(ui_size != Im->ui_size) throw OptiVec_SizeErr();
		VF_PtoReIm(vx_vect, Im->vx_vect, X.vx_vect, ui_size);
	}

	inline void vtbase::PtoReIm(fVecObj Im, cvta X)
	{
		enforceSize();
		if(ui_size != Im.ui_size) throw OptiVec_SizeErr();
		VF_PtoReIm(vx_vect, Im.vx_vect, X.vx_vect, ui_size);
	}

	inline void vtbase::PtoRe(cvta X)
	{
		enforceSize();
		VF_PtoRe(vx_vect, X.vx_vect, ui_size);
	}

	inline void vtbase::PtoIm(cvta X)
	{
		enforceSize();
		VF_PtoIm(vx_vect, X.vx_vect, ui_size);
	}

	inline int vtbase::abs(cvta cplx)
	{
		enforceSize();
		return VPF_abs(vx_vect, cplx.vx_vect, ui_size);
	}
#endif //FVECOBJ_H

#ifdef DVECOBJ_H
#endif //DVECOBJ_H

#ifdef EVECOBJ_H
#if (defined __BORLANDC__) && !(defined _WIN64) //80-bit numbers supported
#endif //__BORLANDC__
#endif //EVECOBJ_H

#ifdef CFVECOBJ_H
	inline void pfVecObj::CtoP(const cfVecObj& X)
	{
		enforceSize();
		VF_CtoP(vx_vect,X.vx_vect,ui_size);
	}

	inline void cfVecObj::PtoC(const pfVecObj& X)
	{
		enforceSize();
		VF_PtoC(vx_vect,X.vx_vect,ui_size);
	}

	inline int cfVecObj::logtoC(const pfVecObj& X)
	{
		enforceSize();
		return VPF_logtoC(vx_vect,X.vx_vect,ui_size);
	}

	inline int cfVecObj::log2toC(const pfVecObj& X)
	{
		enforceSize();
		return VPF_log2toC(vx_vect,X.vx_vect,ui_size);
	}

	inline int cfVecObj::log10toC(const pfVecObj& X)
	{
		enforceSize();
		return VPF_log10toC(vx_vect,X.vx_vect,ui_size);
	}

	inline int pfVecObj::exptoP(const cfVecObj& X)
	{
		enforceSize();
		return VCF_exptoP(vx_vect,X.vx_vect,ui_size);
	}
#endif //CFVECOBJ_H

#ifdef CDVECOBJ_H
#endif //CDVECOBJ_H

#ifdef CEVECOBJ_H
#if (defined __BORLANDC__) && !(defined _WIN64) //80-bit numbers supported
#endif //__BORLANDC__
#endif //CEVECOBJ_H

#ifdef PFVECOBJ_H
#endif //PFVECOBJ_H

#ifdef PDVECOBJ_H
	inline void pfVecObj::PDtoPF(const pdVecObj& X)
	{
		enforceSize();
		V_PDtoPF(vx_vect,X.vx_vect,ui_size);
	}

	inline void pdVecObj::PFtoPD(const pfVecObj& X)
	{
		enforceSize();
		V_PFtoPD(vx_vect,X.vx_vect,ui_size);
	}
#endif //PDVECOBJ_H

#ifdef PEVECOBJ_H
#if (defined __BORLANDC__) && !(defined _WIN64) //80-bit numbers supported
	inline void pfVecObj::PEtoPF(const peVecObj& X)
	{
		enforceSize();
		V_PEtoPF(vx_vect,X.vx_vect,ui_size);
	}

	inline void peVecObj::PFtoPE(const pfVecObj& X)
	{
		enforceSize();
		V_PFtoPE(vx_vect,X.vx_vect,ui_size);
	}
#endif //__BORLANDC__
#endif //PEVECOBJ_H

#if defined _MSC_VER || (defined __BORLANDC__ && __BORLANDC__ >= 0x500)
}  // end of namespace OptiVec
#endif

#undef cvta
#undef cuia
#undef cta

#undef vta
#undef uia
#undef ta

#undef vt
#undef t
#undef tVector

#undef vtbase
#undef tbase

#undef tVecObj
#ifdef __BORLANDC__
#pragma warn .inl
#endif

#define PFVECOBJ_H
#endif //PFVECOBJ_H
