#pragma once

#include <GL/glew.h>
#include <math/assert.h>
#include <sized.h>


namespace gl {
using namespace sized;

enum class Target : GLenum {
	/** Vertex attributes */
	Array = GL_ARRAY_BUFFER,
	/** Atomic counter storage */
	AtomicCounter = GL_ATOMIC_COUNTER_BUFFER,
	/** Buffer copy source */
	CopyRead = GL_COPY_READ_BUFFER,
	/** Buffer copy destination */
	CopyWrite = GL_COPY_WRITE_BUFFER,
	/** Indirect compute dispatch commands */
	DispatchIndirect = GL_DISPATCH_INDIRECT_BUFFER,
	/** Indirect command arguments */
	DrawIndirect = GL_DRAW_INDIRECT_BUFFER,
	/** Vertex array indices */
	ElementArray = GL_ELEMENT_ARRAY_BUFFER,
	/** Pixel read target */
	PixelPack = GL_PIXEL_PACK_BUFFER,
	/** Texture data source */
	PixelUnpack = GL_PIXEL_UNPACK_BUFFER,
	/** Query result buffer */
	Query = GL_QUERY_BUFFER,
	/** Read-write storage for shaders */
	ShaderStorage = GL_SHADER_STORAGE_BUFFER,
	/** Texture data buffer */
	Texture = GL_TEXTURE_BUFFER,
	/** Transform feedback buffer */
	TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,
	/** Uniform block storage */
	Uniform = GL_UNIFORM_BUFFER,
};

enum class Usage : GLenum {
	/**
	 * The data store contents will be modified once and used at most a few
	 * times. The data store contents are modified by the application, and used
	 * as the source for GL drawing and image specification commands.
	 */
	StreamDraw = GL_STREAM_DRAW,
	/**
	 * The data store contents will be modified once and used at most a few
	 * times. The data store contents are modified by reading data from the GL,
	 * and used to return that data when queried by the application.
	 */
	StreamRead = GL_STREAM_READ,
	/**
	 * The data store contents will be modified once and used at most a few
	 * times. The data store contents are modified by reading data from the GL,
	 * and used as the source for GL drawing and image specification commands.
	 */
	StreamCopy = GL_STREAM_COPY,

	/**
	 * The data store contents will be modified once and used many times. The
	 * data store contents are modified by the application, and used as the
	 * source for GL drawing and image specification commands.
	 */
	StaticDraw = GL_STATIC_DRAW,
	/**
	 * The data store contents will be modified once and used many times. The
	 * data store contents are modified by reading data from the GL, and used to
	 * return that data when queried by the application.
	 */
	StaticRead = GL_STATIC_READ,
	/**
	 * The data store contents will be modified once and used many times. The
	 * data store contents are modified by reading data from the GL, and used as
	 * the source for GL drawing and image specification commands.
	 */
	StaticCopy = GL_STATIC_COPY,

	/**
	 * The data store contents will be modified repeatedly and used many times.
	 * The data store contents are modified by the application, and used as the
	 * source for GL drawing and image specification commands.
	 */
	DynamicDraw = GL_DYNAMIC_DRAW,
	/**
	 * The data store contents will be modified repeatedly and used many times.
	 * The data store contents are modified by reading data from the GL, and
	 * used to return that data when queried by the application.
	 */
	DynamicRead = GL_DYNAMIC_READ,
	/**
	 * The data store contents will be modified repeatedly and used many times.
	 * The data store contents are modified by reading data from the GL, and
	 * used as the source for GL drawing and image specification commands.
	 */
	DynamicCopy = GL_DYNAMIC_COPY,
};

class Mask {
public:
	enum Bits : u32 {
		ColorBuffer = GL_COLOR_BUFFER_BIT,
		DepthBuffer = GL_DEPTH_BUFFER_BIT,
		StencilBuffer = GL_STENCIL_BUFFER_BIT,
	};
};

enum class DrawMode : GLenum {
	Points = GL_POINTS,
	LineStrip = GL_LINE_STRIP,
	LineLoop = GL_LINE_LOOP,
	Lines = GL_LINES,
	LineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
	LinesAdjacency = GL_LINES_ADJACENCY,
	TriangleStrip = GL_TRIANGLE_STRIP,
	TriangleFan = GL_TRIANGLE_FAN,
	Triangles = GL_TRIANGLES,
	TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
	TrianglesAdjacency = GL_TRIANGLES_ADJACENCY,
	Patches = GL_PATCHES,
};

enum class Info : GLenum {
	Vendor = GL_VENDOR,
	Renderer = GL_RENDERER,
	Version = GL_VERSION,
	ShadingLanguageVersion = GL_SHADING_LANGUAGE_VERSION,
	Extensions = GL_EXTENSIONS,
};

enum class Scalar : GLenum {
	Byte = GL_BYTE,   UnsignedByte = GL_UNSIGNED_BYTE,
	Short = GL_SHORT, UnsignedShort = GL_UNSIGNED_SHORT,
	Int = GL_INT,     UnsignedInt = GL_UNSIGNED_INT,
	HalfFloat = GL_HALF_FLOAT,
	Float = GL_FLOAT,
	Double = GL_DOUBLE,
	Fixed = GL_FIXED,
	Int_2_10_10_10_Rev = GL_INT_2_10_10_10_REV,
	UnsignedInt_2_10_10_10_Rev = GL_UNSIGNED_INT_2_10_10_10_REV,
	UnsignedInt_10f_11f_11f_Rev = GL_UNSIGNED_INT_10F_11F_11F_REV,

	i8 = Byte,   u8 = UnsignedByte,
	i16 = Short, u16 = UnsignedShort,
	i32 = Int,   u32 = UnsignedInt,
	f16 = HalfFloat,
	f32 = Float,
	f64 = Double,
	fixed = Fixed,
};

template <typename T>
auto type() -> Scalar;

template <> inline auto type<i8>() -> Scalar { return Scalar::i8; }
template <> inline auto type<u8>() -> Scalar { return Scalar::u8; }
template <> inline auto type<i16>() -> Scalar { return Scalar::i16; }
template <> inline auto type<u16>() -> Scalar { return Scalar::u16; }
template <> inline auto type<i32>() -> Scalar { return Scalar::i32; }
template <> inline auto type<u32>() -> Scalar { return Scalar::u32; }
template <> inline auto type<f32>() -> Scalar { return Scalar::f32; }
template <> inline auto type<f64>() -> Scalar { return Scalar::f64; }

template <Scalar T>
auto size() -> usize;

template <> inline auto size<Scalar::i8>()  -> usize { return sizeof(i8); }
template <> inline auto size<Scalar::u8>()  -> usize { return sizeof(u8); }
template <> inline auto size<Scalar::i16>() -> usize { return sizeof(i16); }
template <> inline auto size<Scalar::u16>() -> usize { return sizeof(u16); }
template <> inline auto size<Scalar::i32>() -> usize { return sizeof(i32); }
template <> inline auto size<Scalar::u32>() -> usize { return sizeof(u32); }
template <> inline auto size<Scalar::f32>() -> usize { return sizeof(f32); }
template <> inline auto size<Scalar::f64>() -> usize { return sizeof(f64); }

inline auto size_of(Scalar type) -> usize
{
	switch (type) {
		case Scalar::i8: return sizeof(i8);
		case Scalar::u8: return sizeof(u8);
		case Scalar::i16: return sizeof(i16);
		case Scalar::u16: return sizeof(u16);
		case Scalar::i32: return sizeof(i32);
		case Scalar::u32: return sizeof(u32);
		case Scalar::f32: return sizeof(f32);
		case Scalar::f64: return sizeof(f64);
		default: {
			ASSERT(false, "gl::size_of not implemented for type: {:x}", static_cast<GLenum>(type));
			return 0;
		}
	}
}

enum class Unsigned : GLenum {
	Byte = GL_UNSIGNED_BYTE,
	Short = GL_UNSIGNED_SHORT,
	Int = GL_UNSIGNED_INT,

	u8 = Byte,
	u16 = Short,
	u32 = Int,
};

enum class Shader : GLenum {
	_none = 0,
	Compute = GL_COMPUTE_SHADER,
	Vertex = GL_VERTEX_SHADER,
	TessControl = GL_TESS_CONTROL_SHADER,
	TessEval = GL_TESS_EVALUATION_SHADER,
	Geometry = GL_GEOMETRY_SHADER,
	Fragment = GL_FRAGMENT_SHADER,
};

enum class ShaderParam : GLenum {
	Type = GL_SHADER_TYPE,
	DeleteStatus = GL_DELETE_STATUS,
	CompileStatus = GL_COMPILE_STATUS,
	InfoLogLength = GL_INFO_LOG_LENGTH,
	SourceLength = GL_SHADER_SOURCE_LENGTH,
};

enum class Result : GLenum {
	OK = GLEW_OK,
	NoGLVersion = GLEW_ERROR_NO_GL_VERSION,
	GLVersion10Only = GLEW_ERROR_GL_VERSION_10_ONLY,
	GLXVersion11Only = GLEW_ERROR_GLX_VERSION_11_ONLY,
	NoGLXDisplay = GLEW_ERROR_NO_GLX_DISPLAY,
};

} // namespace gl
