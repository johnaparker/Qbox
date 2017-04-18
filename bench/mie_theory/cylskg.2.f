c
c reads in a data set with metallic eps.
c
      implicit real*8(a-h,o-z)
      complex*16 ri,refrel, epsc,t1(300), t2(300)
      character*60 sigdat,epsdat
      dimension ang(300)
      ri=(0.d0,1.d0)
      c=2.997925d8
      pi=4*datan(1.d0)
      rnm=1.d-9
c
c
c      refmed=1.5d0
       refmed=1.d0
c
c read in eps file and
c specify radius of cylinder in nm
c
      read(5,*)epsdat,nom
      read(5,*)rad
      rad=rad*1.e-9
c
c read output data file name
      read(5,*)sigdat
      open(unit=7,file=sigdat,status='unknown')
      open(unit=8,file=epsdat,status='old')
c
c
c loop over lambda or freqs :
c
      do 666 iom=1,nom
      read(8,*)rlamb,epsre,epsim
c
      epsc=epsre+ri*epsim
      refrel=cdsqrt(epsc)/refmed
c
      omega=2.d0*pi*c/(rlamb*rnm)
c
c
c
      wavel=2.d0*pi*c/omega
c
      x=2.d0*pi*rad*refmed/wavel
c
      fin=180.d0
c
c      intang=20
c skg change relative to Bohren and Huffman :
      intang=200
c
      call bhcyl(x,refrel,t1,t2,qscpar,qscper,qexpar,qexper,
     > fin,intang,ang)
c
c
c       write(6,*)' omega/10**15 ',omega/1.d15
c       write(6,*)' qscpar, qexpar ',qscpar,qexpar
c       write(6,*)' qscper, qexper ',qscper,qexper
c       write(6,*)' '
c
c write out E field perpendicular to cyl axis info
c 2*rad/1.e-9 converts to nm units 
       qscout=qscpar*2*rad/1.d-9
       qexout= qexpar*2*rad/1.d-9
       qabout=  qexout - qscout 
c
       write(7,1000)rlamb,qscout,qabout,qexout
c
 666   continue
c
 1000   format(1x,4e13.5)
c
        stop
        end
c
c
      subroutine bhcyl(x,refrel,t1,t2,qscpar,qscper,qexpar,qexper,
     > fin,intang,ang)
c
       implicit real*8(a-h,o-z)
       complex*16 refrel,y,an,bn,a0,b0
       complex*16 g(5000),bh(5000),t1(300),t2(300)
       dimension theta(300),ang(300),bj(5000),by(5000),f(5000)
c
      qcspar=0.d0
      qcsper=0.d0
      qexpar=0.d0
      qexper=0.d0
c
      radcon=4.d0*datan(1.d0)/180.d0
c
      y=x*refrel
c      xstop=x+4*x**(0.333333333333333d0) + 2.d0
c      nstop=xstop
c skg change -- include many more terms in iteration
      xstop=5*x
      nstop=xstop
c end of this skg change
c
      ymod=cdabs(y)
      nmx=dmax1(xstop,ymod)+15
      npts=intang+1
      dang=fin/dfloat(intang)
c
      do j=1,npts
      ang(j)=(dfloat(j)-1.d0)*dang
      theta(j)=ang(j)*radcon
      enddo
c
      g(nmx)=dcmplx(0.d0,0.d0)
      nn=nmx-1
c
      do n=1,nn
      rn=nmx-n+1
      k=nmx-n
      g(k)=( (rn-2.d0)/y ) - ( 1.d0/(g(k+1)+(rn-1.d0)/y) )
      enddo
c
c 
      ndelta=(101.d0+x)**(0.499d0)
      mst=nstop+ndelta
c skg comment -- I guess this makes mst "even" :
      mst=(mst/2)*2
c
c top of p. 496 now :
c
      f(mst+1)=0.d0
      f(mst)=1.d-32
      m1=mst-1
c
      do l=1,m1
      ml=mst-l
      f(ml)=2*dfloat(ml) * f(ml+1)/x - f(ml+2)
      enddo
c
      alpha=f(1)
      m2=mst-2
c
      do ll=2,m2,2
      alpha=alpha+2*f(ll+1)
      enddo
c
      m3=m2+1
c
      do n=1,m3
      bj(n)=f(n)/alpha
      enddo
c
      by(1)=bj(1)*(dlog(x/2.d0)+0.577215664d0)
      m4=mst/2-1
c
      do l=1,m4
      by(1)=by(1)-2*((-1.d0)**l)*bj(2*l+1)/dfloat(l)
      enddo
c
      by(1)=0.636619772d0*by(1)
      by(2)=bj(2)*by(1)-0.636619772d0/x
      by(2)=by(2)/bj(1)
c
      ns=nstop-1
c
      do kk=1,ns
      by(kk+2)=2*dfloat(kk)*by(kk+1)/x-by(kk)
      enddo
c
      nn=nstop+1
c
      do n=1,nn
      bh(n)=dcmplx(bj(n),by(n))
      enddo
c
      a0=g(1)*bj(1)/refrel+bj(2)
      a0=a0/(g(1)*bh(1)/refrel+bh(2))
      b0=refrel*g(1)*bj(1)+bj(2)
      b0=b0/(refrel*g(1)*bh(1)+bh(2))     
c
      qscpar=cdabs(b0)*cdabs(b0)
      qscper=cdabs(a0)*cdabs(a0)
c
      do k=1,npts
      t1(k)=b0
      t2(k)=a0
      enddo
c
      do n=1,nstop
      rn=n
      an=(g(n+1)/refrel+rn/x)*bj(n+1)-bj(n)
      an=an/((g(n+1)/refrel+rn/x)*bh(n+1)-bh(n))
      bn=(refrel*g(n+1)+rn/x)*bj(n+1)-bj(n)
      bn=bn/((refrel*g(n+1)+rn/x)*bh(n+1)-bh(n))
c
       do j=1,npts
       c=dcos(rn*theta(j))
       t1(j)=2*bn*c+t1(j)
       t2(j)=2*an*c+t2(j)
       enddo
c
      qscpar=qscpar+2*cdabs(bn)*cdabs(bn)
      qscper=qscper+2*cdabs(an)*cdabs(an)
c
      enddo
c
c
      qscpar=(2.d0/x)*qscpar
      qscper=(2.d0/x)*qscper
      qexper=(2.d0/x)*dreal(t2(1))
      qexpar=(2.d0/x)*dreal(t1(1))
c 
c
c
      return
      end
c
      subroutine drude(omega,epsre,epsim,refre,refim)
      implicit real*8(a-h,o-z)
      complex*16 cc,ri
      ri=(0.d0,1.d0)
c
c Ag parameters, omp and gam in ev :
c      epsinf=7.342d0
c      omp=10.511d0
c      gam=0.0804d0
c D1 param
       epsinf=8.926
       omp=11.585
       gam=0.2030

c
c convert omega to ev :
c
      om=27.2114d0*omega*2.41888d-17
c
      opop=omp*omp
      omom=om*om
c
c the dielectric const :
c
      cc=epsinf - opop/(omom+ri*gam*om)
c
       epsre= dreal(cc)
       epsim= dimag(cc)
c
c now get refractive index :
c
       arg=dsqrt( epsre**2 + epsim**2)
c
       refre=dsqrt( (arg +epsre)/2.d0)
       refim=dsqrt( (arg -epsre)/2.d0)
c 
c        write(6,*)' om/ev ',om
c        write(6,*)' diel ',epsre,epsim
c        write(6,*)' n,k  ',refre,refim
       return
      end
