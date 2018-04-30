#include "OpenGLError.h"

// Reutn false if everything is OK
//static 
bool OpenGLError::bWasThereAnOpenGLError(void)
{
	GLenum errorEnum = GL_NO_ERROR;
	std::string errorText;
	std::string errorDetails;
	return OpenGLError::bWasThereAnOpenGLError(errorEnum, errorText, errorDetails);
}


//static 
bool OpenGLError::bWasThereAnOpenGLError(GLenum &errorEnum)
{
	std::string errorText;
	std::string errorDetails;
	return OpenGLError::bWasThereAnOpenGLError(errorEnum, errorText, errorDetails);
}

//static 
bool OpenGLError::bWasThereAnOpenGLError(GLenum &errorEnum, std::string &errorString)
{
	std::string errorDetails;
	return OpenGLError::bWasThereAnOpenGLError(errorEnum, errorString, errorDetails);
}

//static 
bool OpenGLError::bWasThereAnOpenGLError(std::string &errorText, std::string &errorDetails)
{
#ifdef _DEBUG
	GLenum errorEnum = GL_NO_ERROR;
	return OpenGLError::bWasThereAnOpenGLError(errorEnum, errorText, errorDetails);
#else
	return false;
#endif
}

// These require an error logger class to be passed, too
//static 
bool OpenGLError::LogOpenGLErrorIfPresent(ErrorLog &errorLogger)
{
	std::string errorText;
	std::string errorDetails;
	bool bWasError = OpenGLError::bWasThereAnOpenGLError(errorText, errorDetails);
	if (bWasError)
	{	// Sorry, but this is because the stream operator isn't properly overloaded
		errorLogger << errorText;
		errorLogger << " ";
		errorLogger << errorDetails;
	}
	return bWasError;
}

//static 
bool OpenGLError::LogOpenGLErrorIfPresent(std::string prependText, ErrorLog &errorLogger)
{
	std::string errorText;
	std::string errorDetails;
	bool bWasError = OpenGLError::bWasThereAnOpenGLError(errorText, errorDetails);
	if (bWasError)
	{	// Sorry, but this is because the stream operator isn't properly overloaded
		std::stringstream ssError;
		ssError << prependText << ":" << errorText << "\n\tDetails:" << errorDetails;
		errorLogger.PrintToLog(ssError.str());
	}
	return bWasError;
}

//static 
bool OpenGLError::LogOpenGLErorrIfPresent(const int line, const char* file, ErrorLog &errorLogger)
{
	std::string errorText;
	std::string errorDetails;
	bool bWasError = OpenGLError::bWasThereAnOpenGLError(errorText, errorDetails);
	if (bWasError)
	{	// Sorry, but this is because the stream operator isn't properly overloaded
		std::stringstream ssError;
		ssError << errorText << "\n\tDetails:" << errorDetails;
		errorLogger.PrintToLog(ssError.str(), line, file);
	}
	return bWasError;
}

//static 
bool OpenGLError::LogOpenGLErorrIfPresent(std::string prependText, const int line, const char* file, ErrorLog &errorLogger)
{
	std::string errorText;
	std::string errorDetails;
	bool bWasError = OpenGLError::bWasThereAnOpenGLError(errorText, errorDetails);
	if (bWasError)
	{	// Sorry, but this is because the stream operator isn't properly overloaded
		std::stringstream ssError;
		ssError << prependText << ":" << errorText << "\n\tDetails:" << errorDetails;
		errorLogger.PrintToLog(ssError.str(), line, file);
	}
	return bWasError;
}

//static 
std::string OpenGLError::TranslateErrorEnum(GLenum &errorEnum)
{
	std::string errorDetails;
	return OpenGLError::TranslateErrorEnumDetails(errorEnum, errorDetails);
}

//static 
std::string OpenGLError::TranslateErrorEnumDetails(GLenum &errorEnum, std::string &errorDetails)
{
	// from: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetError.xml
	switch (errorEnum)
	{
	case GL_NO_ERROR:
		errorDetails = "GL_NO_ERROR: No error has been recorded.";
		return "GL_NO_ERROR";
		break;
	case GL_INVALID_ENUM:
		errorDetails = "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
		return "GL_INVALID_ENUM";
		break;
	case GL_INVALID_VALUE:
		errorDetails = "GL_INVALID_VALUE: A numeric argument is out of range.";
		return "GL_INVALID_VALUE";
		break;
	case GL_INVALID_OPERATION:
		errorDetails = "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
		return "GL_INVALID_OPERATION";
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		errorDetails = "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
		return "GL_INVALID_FRAMEBUFFER_OPERATION";
		break;
	case GL_OUT_OF_MEMORY:
		errorDetails = "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
		return "GL_OUT_OF_MEMORY";
		break;
	case GL_STACK_UNDERFLOW:
		errorDetails = "GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow.";
		return "GL_STACK_UNDERFLOW";
		break;
	case GL_STACK_OVERFLOW:
		errorDetails = "GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow.";
		return "GL_STACK_OVERFLOW";
		break;
	default:
		errorDetails = "Unknown error";
		return "Unknown error";
		break;
	};
	// This should be caught by the default...
	errorDetails = "Unknown error";
	return "Unknown error";
}


//static 
bool OpenGLError::bWasThereAnOpenGLError(GLenum &errorEnum, std::string &errorText, std::string &errorDetails)
{
#ifdef _DEBUG		// Visual studio specific
	errorEnum = glGetError();
	errorText = OpenGLError::TranslateErrorEnumDetails(errorEnum, errorDetails);
	if (errorEnum == GL_NO_ERROR)
	{
		return false;	// No error
	}// else
	return true;	// There -WAS- an error
#else
	// DON'T call glGetError() if in release
	errorText = "glGetError() call bypassed in release build";
	errorDetails = "glGetError() call bypassed in release build";

	return false;
#endif
}

