#include "mathlib.h"

//none inline and static functions (mostly Matrix4 functions)
extern const Vector3 ML_A[3] = {Vector3(1.0, 0.0, 0.0), Vector3(0.0, 1.0, 0.0), Vector3(0.0, 0.0, 1.0)};
extern const Vector3 ML_AX = Vector3(1.0, 0.0, 0.0);
extern const Vector3 ML_AY = Vector3(0.0, 1.0, 0.0);
extern const Vector3 ML_AZ = Vector3(0.0, 0.0, 1.0);
extern const Vector3 ML_O = Vector3(0.0, 0.0, 0.0);
extern const Matrix3 ML_M3I = Matrix3(Matrix3::Identity_Matrix);

FTP Matrix2::Identity_Matrix[] =
{
	1.0, 0.0,
	0.0, 1.0
};

FTP Matrix3::Identity_Matrix[] =
{
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0
};

FTP Matrix3::det2x2(FTP a, FTP b, FTP c, FTP d)
{
	return a * d - b * c;
}

FTP Matrix3::det3x3(FTP a1, FTP a2, FTP a3,
                      FTP b1, FTP b2, FTP b3,
                      FTP c1, FTP c2, FTP c3)
{
	return a1*b2*c3 + b1*c2*a3 + c1*a2*b3
		   - a3*b2*c1 - b3*c2*a1 - c3*a2*b1;
}

FTP Matrix3::det3x3(const FTP *m)
{
	return m[0]*m[4]*m[8] + m[1]*m[5]*m[6] + m[2]*m[3]*m[7]
		   - m[6]*m[4]*m[2] - m[7]*m[5]*m[0] - m[8]*m[3]*m[1];
}

Matrix3& Matrix3::adjoint(const Matrix3& other)
{
	FTP a1, a2, a3, b1, b2, b3, c1, c2, c3;
	const FTP *in = other.M;
	FTP *out = M;

	a1 = (in [0 + ( 0 )]);
	b1 = (in [0 + ( 1 )]);
	c1 = (in [0 + ( 2 )]);
	a2 = (in [3 + ( 0 )]);
	b2 = (in [3 + ( 1 )]);
	c2 = (in [3 + ( 2 )]);
	a3 = (in [6 + ( 0 )]);
	b3 = (in [6 + ( 1 )]);
	c3 = (in [6 + ( 2 )]);

	out[0 + ( 0 )]   =   det2x2(b2, c2, b3, c3);
	out[3 + ( 0 )]   = - det2x2(a2, c2, a3, c3);
	out[6 + ( 0 )]   =   det2x2(a2, b2, a3, b3);
	out[0 + ( 1 )]   = - det2x2(b1, c1, b3, c3);
	out[3 + ( 1 )]   =   det2x2(a1, c1, a3, c3);
	out[6 + ( 1 )]   = - det2x2(a1, b1, a3, b3);
	out[0 + ( 2 )]   =   det2x2(b1, c1, b2, c2);
	out[3 + ( 2 )]   = - det2x2(a1, c1, a2, c2);
	out[6 + ( 2 )]   =   det2x2(a1, b1, a2, b2);

	return *this;
}

Matrix3& Matrix3::invert(const Matrix3& other)
{
	int i;
	adjoint(other);
	FTP d = other.det();
	for (i=0; i<9; i++) {
		M[i] = M[i] / d;
	}
	return *this;
}

Matrix3& Matrix3::transpose(const Matrix3& other)
{
	for (int i=0; i<3; i++) {
		for (int j=0; j<3; j++) {
			M[i * 3 + j] = other.M[j * 3 + i];
		}
	}
	return *this;
}

Matrix3& Matrix3::setrotate3d(FTP angle, const Vector3& axis)
{
	Matrix4 rot;
	rot.setrotate(angle,axis);
	set(rot);
	return *this;
}

Matrix3& Matrix3::setreflect3d(const Vector3& p, const Vector3& n)
{
	Matrix4 rot;
	rot.setreflect(p,n);
	set(rot);
	return *this;
}

Matrix3& Matrix3::setreflect3d(const Vector3& n)
{
	Matrix4 rot;
	rot.setreflect(n);
	set(rot);
	return *this;
}

void Matrix3::getaxisangle3d(FTP& angle, Vector3& axis)
{
	FTP dE = std::sqrt(2.0f)/2.0f;
	if (IsEqual(m[0][1],m[1][0],ML_D_L_TOL) && IsEqual(m[0][2],m[2][0],ML_D_L_TOL) && IsEqual(m[1][2],m[2][1],ML_D_L_TOL)) {	// singularity found
		// first check for identity matrix which must have +1 for all terms in leading diagonal and zero in other terms
		if (IsEqual(m[0][1],-m[1][0],ML_D_TOL) && IsEqual(m[0][2],-m[2][0],ML_D_TOL) && IsEqual(m[1][2],-m[2][1],ML_D_TOL) && IsEqual(m[0][0]+m[1][1]+m[2][2],3.0,ML_D_TOL)) {
			// this singularity is identity matrix so angle = 0
			angle = 0;		// zero angle
			axis = ML_AX;	// arbitrary axis
		} else {
			// otherwise this singularity is angle = 180
			angle = 180.0;
			FTP xx = (m[0][0]+1.f)/2.0f;
			FTP yy = (m[1][1]+1.f)/2.0f;
			FTP zz = (m[2][2]+1.f)/2.0f;
			FTP xy = (m[0][1]+m[1][0])/4.0f;
			FTP xz = (m[0][2]+m[2][0])/4.0f;
			FTP yz = (m[1][2]+m[2][1])/4.0f;
			if ((xx>yy) && (xx>zz)) { // m[0][0] is the largest diagonal term
				if (xx < std::numeric_limits<FTP>::epsilon()) {
					axis.x = 0;
					axis.y = dE;
					axis.z = dE;
				} else {
					axis.x = std::sqrt(xx);
					axis.y = xy/axis.x;
					axis.z = xz/axis.x;
				}
			} else if (yy > zz) { // m[1][1] is the largest diagonal term
				if (yy < std::numeric_limits<FTP>::epsilon()) {
					axis.x = dE;
					axis.y = 0;
					axis.z = dE;
				} else {
					axis.y = std::sqrt(yy);
					axis.x = xy/axis.y;
					axis.z = yz/axis.y;
				}	
			} else { // m[2][2] is the largest diagonal term so base result on this
				if (zz< std::numeric_limits<FTP>::epsilon()) {
					axis.x = dE;
					axis.y = dE;
					axis.z = 0;
				} else {
					axis.z = std::sqrt(zz);
					axis.x = xz/axis.z;
					axis.y = yz/axis.z;
				}
			}
		}
	}
	else { // there are no singularities so we can handle normally
		FTP s = std::sqrt((m[2][1]-m[1][2])*(m[2][1]-m[1][2]) + (m[0][2]-m[2][0])*(m[0][2]-m[2][0]) + (m[1][0]-m[0][1])*(m[1][0]-m[0][1])); // used to normalize
		if (IsZero(s)) s=1; 
		// prevent divide by zero, should not happen if matrix is orthogonal and should be caught by singularity test above, but I've left it in just in case
		angle = Acos((m[0][0]+m[1][1]+m[2][2]-1.0)/2.0f);
		axis.x = (m[2][1]-m[1][2])/s;
		axis.y = (m[0][2]-m[2][0])/s;
		axis.z = (m[1][0]-m[0][1])/s;
	}
}

FTP Matrix3::det() const
{
	return det3x3(&M[0]);
}

FTP Matrix3::trace() const
{
	return (M[0] + M[4] + M[8]);
}

FTP Matrix4::Identity_Matrix[] =
{
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};

FTP Matrix4::Orientation_Switch_Matrix[] =
{
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, -1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};

FTP Matrix4::Perspective_Matrix[] =
{
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, -1.0,
	0.0, 0.0, 0.0, 0.0
};

Matrix4& Matrix4::setreflect(const Vector3& n)
{
	M[0] = -2.0f * n.x * n.x + 1.0f;
	M[1] = -2.0f * n.y * n.x;
	M[2] = -2.0f * n.z * n.x;
	M[3] = 0.0f;
	M[4] = -2.0f * n.x * n.y;
	M[5] = -2.0f * n.y * n.y + 1.0f;
	M[6] = -2.0f * n.z * n.y;
	M[7] = 0.0f;
	M[8] = -2.0f * n.x * n.z;
	M[9] = -2.0f * n.y * n.z;
	M[10] = -2.0f * n.z * n.z + 1.0f;
	M[11] = 0.0f;
	M[12] = 0.0f;
	M[13] = 0.0f;
	M[14] = 0.0f;
	M[15] = 1.0f;
	return *this;
}

Matrix4& Matrix4::setreflect(const Vector3& p, const Vector3& n)
{
	FTP d = -n.dot(p);
	M[0] = -2.0f * n.x * n.x + 1.0f;
	M[1] = -2.0f * n.y * n.x;
	M[2] = -2.0f * n.z * n.x;
	M[3] = 0.0f;
	M[4] = -2.0f * n.x * n.y;
	M[5] = -2.0f * n.y * n.y + 1.0f;
	M[6] = -2.0f * n.z * n.y;
	M[7] = 0.0f;
	M[8] = -2.0f * n.x * n.z;
	M[9] = -2.0f * n.y * n.z;
	M[10] = -2.0f * n.z * n.z + 1.0f;
	M[11] = 0.0f;
	M[12] = -2.0f * n.x * d;
	M[13] = -2.0f * n.y * d;
	M[14] = -2.0f * n.z * d;
	M[15] = 1.0f;
	return *this;
}

Matrix4& Matrix4::setrotate(FTP rx, FTP ry, FTP rz)
{
	FTP a = Cos(rx); FTP b = Sin(rx);
	FTP c = Cos(ry); FTP d = Sin(ry);
	FTP e = Cos(rz); FTP f = Sin(rz);
	FTP ad = a * d;
	FTP bd = b * d;

	M[0] =  c * e;
	M[1] = -c * f;
	M[2] = -d;

	M[4] = -bd * e + a * f;
	M[5] =  bd * f + a * e;
	M[6] = -b * c;

	M[8] =  ad * e + b * f;
	M[9] = -ad * f + b * e;
	M[10] =  a * c;

	M[3] = M[7] = M[11] = M[12] = M[13] = M[14] = 0.0;
	M[15] = 1.0;
	return *this;
}

Matrix4& Matrix4::setrotate(FTP angle, FTP x, FTP y, FTP z)
{
	FTP xx, yy, zz, xy, yz, zx, xs, ys, zs, c_complement;
	FTP s = Sin(angle);
	FTP c = Cos(angle);
	FTP magnitude = (FTP)sqrt(x * x + y * y + z * z);
	FTP *data = M;
	if(magnitude == 0.0) {
		setidentity();
		return *this;
	}
	x /= magnitude;
	y /= magnitude;
	z /= magnitude;
	xx = x * x;
	yy = y * y;
	zz = z * z;
	xy = x * y;
	yz = y * z;
	zx = z * x;
	xs = x * s;
	ys = y * s;
	zs = z * s;
	c_complement = 1.0F - c;
	data[0] = (c_complement * xx) + c;
	data[4] = (c_complement * xy) - zs;
	data[8] = (c_complement * zx) + ys;
	data[12] = 0.0F;
	data[1] = (c_complement * xy) + zs;
	data[5] = (c_complement * yy) + c;
	data[9] = (c_complement * yz) - xs;
	data[13] = 0.0F;
	data[2] = (c_complement * zx) - ys;
	data[6] = (c_complement * yz) + xs;
	data[10] = (c_complement * zz) + c;
	data[14] = 0.0F;
	data[3] = 0.0F;
	data[7] = 0.0F;
	data[11] = 0.0F;
	data[15] = 1.0F;
	return *this;
}

FTP Matrix4::det() const
{
	FTP a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;
	a1 = (M [(( 0 ) << 2) + (  0 )]) ;
	b1 = (M [(( 0 ) << 2) + (  1 )]) ; 
	c1 = (M [(( 0 ) << 2) + (  2 )]) ;
	d1 = (M [(( 0 ) << 2) + (  3 )]) ;

	a2 = (M [(( 1 ) << 2) + (  0 )]) ;
	b2 = (M [(( 1 ) << 2) + (  1 )]) ; 
	c2 = (M [(( 1 ) << 2) + (  2 )]) ;
	d2 = (M [(( 1 ) << 2) + (  3 )]) ;

	a3 = (M [(( 2 ) << 2) + (  0 )]) ;
	b3 = (M [(( 2 ) << 2) + (  1 )]) ; 
	c3 = (M [(( 2 ) << 2) + (  2 )]) ;
	d3 = (M [(( 2 ) << 2) + (  3 )]) ;

	a4 = (M [(( 3 ) << 2) + (  0 )]) ;
	b4 = (M [(( 3 ) << 2) + (  1 )]) ; 
	c4 = (M [(( 3 ) << 2) + (  2 )]) ;
	d4 = (M [(( 3 ) << 2) + (  3 )]) ;

	return  a1 * det3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4) -
			b1 * det3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4) +
			c1 * det3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4) -
			d1 * det3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4);
}

Matrix4& Matrix4::adjoint(const Matrix4& other)
{
	FTP a1, a2, a3, a4, b1, b2, b3, b4;
	FTP c1, c2, c3, c4, d1, d2, d3, d4;
	const FTP *in = other.M;
	FTP *out = M;

	a1 = (in [(( 0 ) << 2) + (  0 )]) ; b1 = (in [(( 0 ) << 2) + (  1 )]) ;
	c1 = (in [(( 0 ) << 2) + (  2 )]) ; d1 = (in [(( 0 ) << 2) + (  3 )]) ;
	a2 = (in [(( 1 ) << 2) + (  0 )]) ; b2 = (in [(( 1 ) << 2) + (  1 )]) ;
	c2 = (in [(( 1 ) << 2) + (  2 )]) ; d2 = (in [(( 1 ) << 2) + (  3 )]) ;
	a3 = (in [(( 2 ) << 2) + (  0 )]) ; b3 = (in [(( 2 ) << 2) + (  1 )]) ;
	c3 = (in [(( 2 ) << 2) + (  2 )]) ; d3 = (in [(( 2 ) << 2) + (  3 )]) ;
	a4 = (in [(( 3 ) << 2) + (  0 )]) ; b4 = (in [(( 3 ) << 2) + (  1 )]) ;
	c4 = (in [(( 3 ) << 2) + (  2 )]) ; d4 = (in [(( 3 ) << 2) + (  3 )]) ;

	out[(( 0 ) << 2) + (  0 )]   =   det3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4);
	out[(( 1 ) << 2) + (  0 )]   = - det3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4);
	out[(( 2 ) << 2) + (  0 )]   =   det3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4);
	out[(( 3 ) << 2) + (  0 )]   = - det3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4);

	out[(( 0 ) << 2) + (  1 )]   = - det3x3( b1, b3, b4, c1, c3, c4, d1, d3, d4);
	out[(( 1 ) << 2) + (  1 )]   =   det3x3( a1, a3, a4, c1, c3, c4, d1, d3, d4);
	out[(( 2 ) << 2) + (  1 )]   = - det3x3( a1, a3, a4, b1, b3, b4, d1, d3, d4);
	out[(( 3 ) << 2) + (  1 )]   =   det3x3( a1, a3, a4, b1, b3, b4, c1, c3, c4);

	out[(( 0 ) << 2) + (  2 )]   =   det3x3( b1, b2, b4, c1, c2, c4, d1, d2, d4);
	out[(( 1 ) << 2) + (  2 )]   = - det3x3( a1, a2, a4, c1, c2, c4, d1, d2, d4);
	out[(( 2 ) << 2) + (  2 )]   =   det3x3( a1, a2, a4, b1, b2, b4, d1, d2, d4);
	out[(( 3 ) << 2) + (  2 )]   = - det3x3( a1, a2, a4, b1, b2, b4, c1, c2, c4);

	out[(( 0 ) << 2) + (  3 )]   = - det3x3( b1, b2, b3, c1, c2, c3, d1, d2, d3);
	out[(( 1 ) << 2) + (  3 )]   =   det3x3( a1, a2, a3, c1, c2, c3, d1, d2, d3);
	out[(( 2 ) << 2) + (  3 )]   = - det3x3( a1, a2, a3, b1, b2, b3, d1, d2, d3);
	out[(( 3 ) << 2) + (  3 )]   =   det3x3( a1, a2, a3, b1, b2, b3, c1, c2, c3);
	return *this;
}

Matrix4& Matrix4::transpose(const Matrix4& other)
{
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++)
			M[i * 4 + j] = other.M[j * 4 + i];
	}
	return *this;
}

Matrix4& Matrix4::invert(const Matrix4& other)
{
	int i;
	adjoint(other);
	FTP d = other.det();
	for (i = 0; i < 16; i++) M[i] = M[i] / d;
	return *this;
}

Matrix4& Matrix4::setprojection(FTP fov, FTP aspect, FTP znear, FTP zfar)
{
	FTP top = znear * Tan(fov);
	FTP bottom = -top;
	FTP left = bottom * aspect;
	FTP right = top * aspect;
	FTP x = (2.0f * znear) / (right - left);
	FTP y = (2.0f * znear) / (top - bottom);
	FTP a = (right + left) / (right - left);
	FTP b = (top + bottom) / (top - bottom);
	FTP c = -(zfar + znear) / (zfar - znear);
	FTP d = -(2.0f * zfar * znear) / (zfar - znear);
	M[0]  = x;     M[1]  = 0.0f;  M[2]  = 0.0f; M[3]  = 0.0f;
	M[4]  = 0.0f;  M[5]  = y;     M[6]  = 0.0f; M[7]  = 0.0f;
	M[8]  = a;     M[9]  = b;     M[10] = c;    M[11] = -1.0f;
	M[12] = 0.0f;  M[13] = 0.0f;  M[14] = d;    M[15] = 0.0f;
	return *this;
}

Matrix4& Matrix4::setothogonal(FTP znear, FTP zfar)
{
	FTP x, y, z;
	FTP tx, ty, tz;
	FTP sml = 0.0f;
	x = 2.0f / (1.0f + sml);
	y = 2.0f / (1.0f + sml);
	z = -2.0f / (zfar - znear);
	tx = -(1.0f - sml) / (1.0f + sml);
	ty = -(1.0f - sml) / (1.0f + sml);
	tz = -(zfar + znear) / (zfar - znear);
	M[0] = x;    M[4] = 0.0f;  M[8]  = 0.0f;  M[12] = tx;
	M[1] = 0.0f; M[5] = y;     M[9]  = 0.0f;  M[13] = ty;
	M[2] = 0.0f; M[6] = 0.0f;  M[10] = z;     M[14] = tz;
	M[3] = 0.0f; M[7] = 0.0f;  M[11] = 0.0f;  M[15] = 1.0f;
	return *this;
}

FTP Matrix4::det2x2(FTP a, FTP b, FTP c, FTP d)
{
	return a * d - b * c;
}

FTP Matrix4::det3x3(FTP a1, FTP a2, FTP a3,
					  FTP b1, FTP b2, FTP b3,
					  FTP c1, FTP c2, FTP c3)
{
	return a1 * det2x2(b2, c2, b3, c3)
		 - b1 * det2x2(a2, c2, a3, c3)
		 + c1 * det2x2(a2, b2, a3, b3);
}

void Solve3x3LinSysDoolittle(FTP a[][3], FTP b[], FTP x[])
{
	FTP A[3][3];
	for (int i=0; i<3; i++) {
		for (int j=0; j<3; j++) {
			A[i][j] = a[i][j];
		}
	}
	// 利用选主元的Doolittle方法求解3x3线性系统
	if (Abs(A[1][0])>Abs(A[2][0]) && Abs(A[1][0])>Abs(A[0][0]))				// the 2nd row is principal row
	{
		if (IsZero(A[1][0])) {
		//	AfxMessageBox("Solve3x3LinSysDoolittle: no unique solution!");
			exit(1);
		} else {
			// exchange row
			FTP tmp1 = A[0][0];
			FTP tmp2 = A[0][1];
			FTP tmp3 = A[0][2];
			FTP tmp4 = b[0];
			A[0][0] = A[1][0]; A[0][1] = A[1][1]; A[0][2] = A[1][2]; b[0] = b[1];
			A[1][0] = tmp1; A[1][1] = tmp2; A[1][2] = tmp3; b[1] = tmp4;
		}
	}
	else if (Abs(A[2][0])>Abs(A[1][0]) && Abs(A[2][0])>Abs(A[0][0]))		// the 3rd row is principal row
	{
		if (IsZero(A[2][0])) {
		//	AfxMessageBox("Solve3x3LinSysDoolittle: no unique solution!");
			exit(1);
		} else {
			// exchange row
			FTP tmp1 = A[0][0];
			FTP tmp2 = A[0][1];
			FTP tmp3 = A[0][2];
			FTP tmp4 = b[0];
			A[0][0] = A[2][0]; A[0][1] = A[2][1]; A[0][2] = A[2][2]; b[0] = b[2];
			A[2][0] = tmp1; A[2][1] = tmp2; A[2][2] = tmp3; b[2] = tmp4;
		}
	}
	
	A[1][0] = A[1][0] / A[0][0];
	A[2][0] = A[2][0] / A[0][0];
	// 选主元
	if (Abs(A[2][1] - A[2][0]*A[0][1]) > Abs(A[1][1] - A[1][0]*A[0][1])) {
		if (IsEqual(A[2][1], A[2][0]*A[0][1])) {
		//	AfxMessageBox("Solve3x3LinSysDoolittle: no unique solution!");
			exit(1);
		} else {
			// exchange row
			FTP tmp1 = A[1][0];
			FTP tmp2 = A[1][1];
			FTP tmp3 = A[1][2];
			FTP tmp4 = b[1];
			A[1][0] = A[2][0]; A[1][1] = A[2][1]; A[1][2] = A[2][2]; b[1] = b[2];
			A[2][0] = tmp1; A[2][1] = tmp2; A[2][2] = tmp3; b[2] = tmp4;
		} 
	} else if (IsEqual(A[1][1], A[1][0]*A[0][1])) {
	//	AfxMessageBox("Solve3x3LinSysDoolittle: no unique solution!");
		exit(1);
	}
	A[1][1] = A[1][1] - A[1][0]*A[0][1];
	A[1][2] = A[1][2] - A[1][0]*A[0][2];
	A[2][1] = (A[2][1] - A[2][0]*A[0][1]) / A[1][1];
	A[2][2] = A[2][2] - A[2][0]*A[0][2] - A[2][1]*A[1][2];

	x[0] = b[0];
	x[1] = b[1] - A[1][0]*x[0];
	x[2] = b[2] - A[2][0]*x[0] - A[2][1]*x[1];
	x[2] = x[2] / A[2][2];
	x[1] = (x[1] - A[1][2]*x[2]) / A[1][1];
	x[0] = (x[0] - A[0][1]*x[1] - A[0][2]*x[2]) / A[0][0];
}

void Solve3x3LinSysGaussElim(FTP a[][3], FTP b[], FTP x[])
{
	int r[3];
	FTP s[3];
	FTP c[3];
	FTP A[3][3];
	int i, j, k;

	for (i=0;i<3;i++)
	{
		for (j=0;j<3;j++)
		{
			A[i][j] = a[i][j];
		}
	}

	for (i=0;i<3;i++)
	{
		// 行指示向量r置初值
		r[i] = i;

		// 每一行中找出s值
		s[i] = fabs(A[i][0]);
		for (j=1;j<3;j++)
		{
			if (s[i] < fabs(A[i][j]))
			{
				s[i] = fabs(A[i][j]);
			}
		}
	}

	for (k=0;k<2;k++)
	{
		// 在第k列中选主元
		c[k] = fabs(A[k][k]) / s[k];
		int R = k;
		int temp;
		for (i=k;i<3;i++)
		{
			if (c[k] < fabs(A[i][k]) / s[k])
			{
				c[k] = fabs(A[i][k]) / s[k];
				R = i;
			}
		}

		if (c[k] == 0.0)
		{
			printf("Error: No unique solution exists!\n");
			exit(-1);
		}

		if (r[R] != r[k])
		{
			temp = r[R];
			r[R] = r[k];
			r[k] = temp;
		}

		for (i=k + 1;i<3;i++)
		{
			FTP m = A[r[i]][k] / A[r[k]][k];
			for (j=k + 1;j<3;j++)
			{
				A[r[i]][j] -= m * A[r[k]][j];
			}
			b[r[i]] -= m * b[r[k]];
		}
	}

	if (A[r[2]][2] == 0.0)
	{
		printf("Error: No unique solution exists!\n");
		exit(-1);
	}

	// 开始回代
	x[2] = b[r[2]] / A[r[2]][2];

	for (i=1;i>=0;i--)
	{
		x[i] = b[r[i]];
		for (j=i + 1;j<3;j++)
		{
			x[i] -= A[r[i]][j] * x[j];
		}
		x[i] /= A[r[i]][i];
	}
}

FTP TriArea(const Vector3 &a, const Vector3 &b, const Vector3 &c)
{
	FTP xy = -b.x*a.y + c.x*a.y + a.x*b.y - c.x*b.y - a.x*c.y + b.x*c.y;
	FTP yz = -b.y*a.z + c.y*a.z + a.y*b.z - c.y*b.z - a.y*c.z + b.y*c.z;
	FTP zx = -b.z*a.x + c.z*a.x + a.z*b.x - c.z*b.x - a.z*c.x + b.z*c.x;
	return 0.5f * (FTP)sqrt(xy*xy + yz*yz + zx*zx);
}