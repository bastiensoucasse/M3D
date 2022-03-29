
#include "Binding_pch.h"


using namespace gl;


namespace glbinding
{


Function<void, GLDEBUGPROC, const void *> Binding::DebugMessageCallback("glDebugMessageCallback");
Function<void, GLDEBUGPROCAMD, void *> Binding::DebugMessageCallbackAMD("glDebugMessageCallbackAMD");
Function<void, GLDEBUGPROCARB, const void *> Binding::DebugMessageCallbackARB("glDebugMessageCallbackARB");
Function<void, GLenum, GLenum, GLenum, GLsizei, const GLuint *, GLboolean> Binding::DebugMessageControl("glDebugMessageControl");
Function<void, GLenum, GLenum, GLenum, GLsizei, const GLuint *, GLboolean> Binding::DebugMessageControlARB("glDebugMessageControlARB");
Function<void, GLenum, GLenum, GLsizei, const GLuint *, GLboolean> Binding::DebugMessageEnableAMD("glDebugMessageEnableAMD");
Function<void, GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar *> Binding::DebugMessageInsert("glDebugMessageInsert");
Function<void, GLenum, GLenum, GLuint, GLsizei, const GLchar *> Binding::DebugMessageInsertAMD("glDebugMessageInsertAMD");
Function<void, GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar *> Binding::DebugMessageInsertARB("glDebugMessageInsertARB");
Function<void, FfdMaskSGIX> Binding::DeformSGIX("glDeformSGIX");
Function<void, GLenum, GLdouble, GLdouble, GLint, GLint, GLdouble, GLdouble, GLint, GLint, GLdouble, GLdouble, GLint, GLint, const GLdouble *> Binding::DeformationMap3dSGIX("glDeformationMap3dSGIX");
Function<void, GLenum, GLfloat, GLfloat, GLint, GLint, GLfloat, GLfloat, GLint, GLint, GLfloat, GLfloat, GLint, GLint, const GLfloat *> Binding::DeformationMap3fSGIX("glDeformationMap3fSGIX");
Function<void, GLuint, GLsizei> Binding::DeleteAsyncMarkersSGIX("glDeleteAsyncMarkersSGIX");
Function<void, GLsizei, const GLuint *> Binding::DeleteBuffers("glDeleteBuffers");
Function<void, GLsizei, const GLuint *> Binding::DeleteBuffersARB("glDeleteBuffersARB");
Function<void, GLsizei, const GLuint *> Binding::DeleteCommandListsNV("glDeleteCommandListsNV");
Function<void, GLsizei, const GLuint *> Binding::DeleteFencesAPPLE("glDeleteFencesAPPLE");
Function<void, GLsizei, const GLuint *> Binding::DeleteFencesNV("glDeleteFencesNV");
Function<void, GLuint> Binding::DeleteFragmentShaderATI("glDeleteFragmentShaderATI");
Function<void, GLsizei, const GLuint *> Binding::DeleteFramebuffers("glDeleteFramebuffers");
Function<void, GLsizei, const GLuint *> Binding::DeleteFramebuffersEXT("glDeleteFramebuffersEXT");
Function<void, GLuint, GLsizei> Binding::DeleteLists("glDeleteLists");
Function<void, GLsizei, const GLuint *> Binding::DeleteMemoryObjectsEXT("glDeleteMemoryObjectsEXT");
Function<void, GLint, const GLchar *> Binding::DeleteNamedStringARB("glDeleteNamedStringARB");
Function<void, GLenum, GLuint, const GLuint *> Binding::DeleteNamesAMD("glDeleteNamesAMD");
Function<void, GLhandleARB> Binding::DeleteObjectARB("glDeleteObjectARB");
Function<void, GLsizei, const GLuint *> Binding::DeleteOcclusionQueriesNV("glDeleteOcclusionQueriesNV");
Function<void, GLuint, GLsizei> Binding::DeletePathsNV("glDeletePathsNV");
Function<void, GLsizei, GLuint *> Binding::DeletePerfMonitorsAMD("glDeletePerfMonitorsAMD");
Function<void, GLuint> Binding::DeletePerfQueryINTEL("glDeletePerfQueryINTEL");
Function<void, GLuint> Binding::DeleteProgram("glDeleteProgram");
Function<void, GLsizei, const GLuint *> Binding::DeleteProgramPipelines("glDeleteProgramPipelines");
Function<void, GLsizei, const GLuint *> Binding::DeleteProgramsARB("glDeleteProgramsARB");
Function<void, GLsizei, const GLuint *> Binding::DeleteProgramsNV("glDeleteProgramsNV");
Function<void, GLsizei, const GLuint *> Binding::DeleteQueries("glDeleteQueries");
Function<void, GLsizei, const GLuint *> Binding::DeleteQueriesARB("glDeleteQueriesARB");
Function<void, GLsizei, const GLint *> Binding::DeleteQueryResourceTagNV("glDeleteQueryResourceTagNV");
Function<void, GLsizei, const GLuint *> Binding::DeleteRenderbuffers("glDeleteRenderbuffers");
Function<void, GLsizei, const GLuint *> Binding::DeleteRenderbuffersEXT("glDeleteRenderbuffersEXT");
Function<void, GLsizei, const GLuint *> Binding::DeleteSamplers("glDeleteSamplers");
Function<void, GLsizei, const GLuint *> Binding::DeleteSemaphoresEXT("glDeleteSemaphoresEXT");
Function<void, GLuint> Binding::DeleteShader("glDeleteShader");
Function<void, GLsizei, const GLuint *> Binding::DeleteStatesNV("glDeleteStatesNV");
Function<void, GLsync> Binding::DeleteSync("glDeleteSync");
Function<void, GLsizei, const GLuint *> Binding::DeleteTextures("glDeleteTextures");
Function<void, GLsizei, const GLuint *> Binding::DeleteTexturesEXT("glDeleteTexturesEXT");
Function<void, GLsizei, const GLuint *> Binding::DeleteTransformFeedbacks("glDeleteTransformFeedbacks");
Function<void, GLsizei, const GLuint *> Binding::DeleteTransformFeedbacksNV("glDeleteTransformFeedbacksNV");
Function<void, GLsizei, const GLuint *> Binding::DeleteVertexArrays("glDeleteVertexArrays");
Function<void, GLsizei, const GLuint *> Binding::DeleteVertexArraysAPPLE("glDeleteVertexArraysAPPLE");
Function<void, GLuint> Binding::DeleteVertexShaderEXT("glDeleteVertexShaderEXT");
Function<void, GLclampd, GLclampd> Binding::DepthBoundsEXT("glDepthBoundsEXT");
Function<void, GLdouble, GLdouble> Binding::DepthBoundsdNV("glDepthBoundsdNV");
Function<void, GLenum> Binding::DepthFunc("glDepthFunc");
Function<void, GLboolean> Binding::DepthMask("glDepthMask");
Function<void, GLdouble, GLdouble> Binding::DepthRange("glDepthRange");
Function<void, GLuint, GLsizei, const GLdouble *> Binding::DepthRangeArrayv("glDepthRangeArrayv");
Function<void, GLuint, GLdouble, GLdouble> Binding::DepthRangeIndexed("glDepthRangeIndexed");
Function<void, GLdouble, GLdouble> Binding::DepthRangedNV("glDepthRangedNV");
Function<void, GLfloat, GLfloat> Binding::DepthRangef("glDepthRangef");
Function<void, GLclampf, GLclampf> Binding::DepthRangefOES("glDepthRangefOES");
Function<void, GLfixed, GLfixed> Binding::DepthRangexOES("glDepthRangexOES");
Function<void, GLhandleARB, GLhandleARB> Binding::DetachObjectARB("glDetachObjectARB");
Function<void, GLuint, GLuint> Binding::DetachShader("glDetachShader");
Function<void, GLenum, GLsizei, const GLfloat *> Binding::DetailTexFuncSGIS("glDetailTexFuncSGIS");
Function<void, GLenum> Binding::Disable("glDisable");
Function<void, GLenum> Binding::DisableClientState("glDisableClientState");
Function<void, GLenum, GLuint> Binding::DisableClientStateIndexedEXT("glDisableClientStateIndexedEXT");
Function<void, GLenum, GLuint> Binding::DisableClientStateiEXT("glDisableClientStateiEXT");
Function<void, GLenum, GLuint> Binding::DisableIndexedEXT("glDisableIndexedEXT");
Function<void, GLuint> Binding::DisableVariantClientStateEXT("glDisableVariantClientStateEXT");
Function<void, GLuint, GLuint> Binding::DisableVertexArrayAttrib("glDisableVertexArrayAttrib");
Function<void, GLuint, GLuint> Binding::DisableVertexArrayAttribEXT("glDisableVertexArrayAttribEXT");
Function<void, GLuint, GLenum> Binding::DisableVertexArrayEXT("glDisableVertexArrayEXT");
Function<void, GLuint, GLenum> Binding::DisableVertexAttribAPPLE("glDisableVertexAttribAPPLE");
Function<void, GLuint> Binding::DisableVertexAttribArray("glDisableVertexAttribArray");
Function<void, GLuint> Binding::DisableVertexAttribArrayARB("glDisableVertexAttribArrayARB");
Function<void, GLenum, GLuint> Binding::Disablei("glDisablei");
Function<void, GLuint, GLuint, GLuint> Binding::DispatchCompute("glDispatchCompute");
Function<void, GLuint, GLuint, GLuint, GLuint, GLuint, GLuint> Binding::DispatchComputeGroupSizeARB("glDispatchComputeGroupSizeARB");
Function<void, GLintptr> Binding::DispatchComputeIndirect("glDispatchComputeIndirect");
Function<void, GLenum, GLint, GLsizei> Binding::DrawArrays("glDrawArrays");
Function<void, GLenum, GLint, GLsizei> Binding::DrawArraysEXT("glDrawArraysEXT");
Function<void, GLenum, const void *> Binding::DrawArraysIndirect("glDrawArraysIndirect");
Function<void, GLenum, GLint, GLsizei, GLsizei> Binding::DrawArraysInstanced("glDrawArraysInstanced");
Function<void, GLenum, GLint, GLsizei, GLsizei> Binding::DrawArraysInstancedARB("glDrawArraysInstancedARB");
Function<void, GLenum, GLint, GLsizei, GLsizei, GLuint> Binding::DrawArraysInstancedBaseInstance("glDrawArraysInstancedBaseInstance");
Function<void, GLenum, GLint, GLsizei, GLsizei> Binding::DrawArraysInstancedEXT("glDrawArraysInstancedEXT");
Function<void, GLenum> Binding::DrawBuffer("glDrawBuffer");
Function<void, GLsizei, const GLenum *> Binding::DrawBuffers("glDrawBuffers");
Function<void, GLsizei, const GLenum *> Binding::DrawBuffersARB("glDrawBuffersARB");
Function<void, GLsizei, const GLenum *> Binding::DrawBuffersATI("glDrawBuffersATI");
Function<void, GLenum, const GLuint64 *, const GLsizei *, GLuint> Binding::DrawCommandsAddressNV("glDrawCommandsAddressNV");
Function<void, GLenum, GLuint, const GLintptr *, const GLsizei *, GLuint> Binding::DrawCommandsNV("glDrawCommandsNV");
Function<void, const GLuint64 *, const GLsizei *, const GLuint *, const GLuint *, GLuint> Binding::DrawCommandsStatesAddressNV("glDrawCommandsStatesAddressNV");
Function<void, GLuint, const GLintptr *, const GLsizei *, const GLuint *, const GLuint *, GLuint> Binding::DrawCommandsStatesNV("glDrawCommandsStatesNV");
Function<void, GLenum, GLint, GLsizei> Binding::DrawElementArrayAPPLE("glDrawElementArrayAPPLE");
Function<void, GLenum, GLsizei> Binding::DrawElementArrayATI("glDrawElementArrayATI");
Function<void, GLenum, GLsizei, GLenum, const void *> Binding::DrawElements("glDrawElements");
Function<void, GLenum, GLsizei, GLenum, const void *, GLint> Binding::DrawElementsBaseVertex("glDrawElementsBaseVertex");
Function<void, GLenum, GLenum, const void *> Binding::DrawElementsIndirect("glDrawElementsIndirect");
Function<void, GLenum, GLsizei, GLenum, const void *, GLsizei> Binding::DrawElementsInstanced("glDrawElementsInstanced");
Function<void, GLenum, GLsizei, GLenum, const void *, GLsizei> Binding::DrawElementsInstancedARB("glDrawElementsInstancedARB");
Function<void, GLenum, GLsizei, GLenum, const void *, GLsizei, GLuint> Binding::DrawElementsInstancedBaseInstance("glDrawElementsInstancedBaseInstance");
Function<void, GLenum, GLsizei, GLenum, const void *, GLsizei, GLint> Binding::DrawElementsInstancedBaseVertex("glDrawElementsInstancedBaseVertex");
Function<void, GLenum, GLsizei, GLenum, const void *, GLsizei, GLint, GLuint> Binding::DrawElementsInstancedBaseVertexBaseInstance("glDrawElementsInstancedBaseVertexBaseInstance");
Function<void, GLenum, GLsizei, GLenum, const void *, GLsizei> Binding::DrawElementsInstancedEXT("glDrawElementsInstancedEXT");
Function<void, GLenum, GLint, GLsizei, GLsizei> Binding::DrawMeshArraysSUN("glDrawMeshArraysSUN");
Function<void, GLintptr> Binding::DrawMeshTasksIndirectNV("glDrawMeshTasksIndirectNV");
Function<void, GLuint, GLuint> Binding::DrawMeshTasksNV("glDrawMeshTasksNV");
Function<void, GLsizei, GLsizei, GLenum, GLenum, const void *> Binding::DrawPixels("glDrawPixels");
Function<void, GLenum, GLuint, GLuint, GLint, GLsizei> Binding::DrawRangeElementArrayAPPLE("glDrawRangeElementArrayAPPLE");
Function<void, GLenum, GLuint, GLuint, GLsizei> Binding::DrawRangeElementArrayATI("glDrawRangeElementArrayATI");
Function<void, GLenum, GLuint, GLuint, GLsizei, GLenum, const void *> Binding::DrawRangeElements("glDrawRangeElements");
Function<void, GLenum, GLuint, GLuint, GLsizei, GLenum, const void *, GLint> Binding::DrawRangeElementsBaseVertex("glDrawRangeElementsBaseVertex");
Function<void, GLenum, GLuint, GLuint, GLsizei, GLenum, const void *> Binding::DrawRangeElementsEXT("glDrawRangeElementsEXT");
Function<void, GLuint, GLuint, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat> Binding::DrawTextureNV("glDrawTextureNV");
Function<void, GLenum, GLuint> Binding::DrawTransformFeedback("glDrawTransformFeedback");
Function<void, GLenum, GLuint, GLsizei> Binding::DrawTransformFeedbackInstanced("glDrawTransformFeedbackInstanced");
Function<void, GLenum, GLuint> Binding::DrawTransformFeedbackNV("glDrawTransformFeedbackNV");
Function<void, GLenum, GLuint, GLuint> Binding::DrawTransformFeedbackStream("glDrawTransformFeedbackStream");
Function<void, GLenum, GLuint, GLuint, GLsizei> Binding::DrawTransformFeedbackStreamInstanced("glDrawTransformFeedbackStreamInstanced");
Function<void, GLuint64, GLuint, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat> Binding::DrawVkImageNV("glDrawVkImageNV");



} // namespace glbinding