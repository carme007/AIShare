#ifndef __TENGINE_OBJECT__
#define __TENGINE_OBJECT__


#include "TMath_Vector3.h"
#include "TMath_Matrix4.h"
#include "TMath_Quaternion.h"
#include "TUtility_Array.h"

namespace TsiU
{
	enum ObjectControlFlag_t
	{
		E_OCF_Active = (1<<0),
		E_OCF_Drawable = (1<<1),
		E_OCF_Show = (1<<2),
	};

	enum{
		EZOrder_Top = 0,
		EZOrder_Bottom = 15,
		EZOrder_Max
	};

	class Object
	{
	public:
		Object()
			:m_vPos(Vec3::ZERO)
			,m_qRotation(Quat::IDENTITY)
			,m_vScale(Vec3(1.f,1.f,1.f))
			,m_Parent(NULL)
			,m_mMatrix(Mat4::ZERO)
			,m_bNeedRecalc(true)
			,m_ZOrder(EZOrder_Top)
		{
			AddControlFlag(E_OCF_Active);
		}
		virtual ~Object(){};

		virtual void Create() = 0;
		virtual void Tick(f32 _fDeltaTime) = 0;

		inline const Vec3& GetPosition()  const { return m_vPos;		}
		inline const Quat& GetRotation()  const { return m_qRotation;	}
		inline const Mat4& GetMatrix()    const { return m_mMatrix;		}
		inline const Vec3& GetScale()	    const { return m_vScale;		}

		inline void SetPosition(const Vec3& v)				{ m_vPos = v;					InvalidateMat();	}
		inline void SetPosition(f32 x, f32 y, f32 z)		{ SetPosition(Vec3(x, y, z));	InvalidateMat();	}
		inline void SetRotation(const Quat& q)				{ m_qRotation = q;				InvalidateMat();	}
		inline void SetMatrix(const Mat4& m)				{ m_mMatrix = m;				InvalidateMat();	}
		inline void SetScale(const Vec3& s)					{ m_vScale = s;					InvalidateMat();	}

		inline void AddControlFlag(u32 _uiFlag)				{ m_uiControlFlags |= (_uiFlag);			}
		inline void RemoveControlFlag(u32 _uiFlag)			{ m_uiControlFlags &= (~_uiFlag);			}
		inline Bool HasControlFlag(u32 _uiFlag)		const	{ return (m_uiControlFlags & _uiFlag) != 0;	}

		inline void SetParent(Object* _poParent)			{ m_Parent = _poParent;}
		inline void AddChild(Object* _poChild)				{ m_poChildList.PushBack(_poChild); _poChild->SetParent(this);	}
		inline void SetZOrder(u8 _zOrder)					{ m_ZOrder = _zOrder;	}
		inline u8 GetZOrder() const							{ return m_ZOrder;		}

		inline Bool IsMatOutOfDate()	const {	return m_bNeedRecalc;	}

		void InvalidateMat();
		Mat4& UpdateMatrix();

	protected:
		Bool	m_bNeedRecalc;

		Vec3	m_vPos;
		Vec3	m_vScale;
		Quat	m_qRotation;
		Mat4	m_mMatrix;

		Array<Object*> m_poChildList;
		Object*	m_Parent;

		u32		m_uiControlFlags;
		u8		m_ZOrder;
	};

	class DrawableObject : public Object
	{
	public:
		DrawableObject()
			:Object()
		{
			AddControlFlag(E_OCF_Drawable | E_OCF_Show);
		}

		virtual void Draw() = 0;
	};
}

#endif