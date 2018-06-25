#ifndef __ShapeBase_H__
#define __ShapeBase_H__

namespace minor
{
	class ShapeBase
	{
	public:
		Shader* _shader;

		static ShapeBase* create()
		{
			// do something for example
			// ShapeBase* base = new ShapeBase();
			// base.allot();
			// base.interpret();
			// base.unbind();
			// return base;

			return nullptr;
		}

		virtual void allot() {};
		virtual void interpret() {};
		virtual void unbind() {};

	protected:
		unsigned int _vao;
		unsigned int _vbo;

		int _verSize;
		float* _vertices;
	};
};

#endif // !__ShapeBase_H__
