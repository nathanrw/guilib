#if !defined(NUI_ERROR_CODE_H) && !defined(NUI_ERROR_CODE_BEGIN)
#define NUI_ERROR_CODE_H
#define NUI_ERROR_CODE_FIRST
#endif

#if defined(NUI_ERROR_CODE_FIRST) || defined(NUI_ERROR_CODE_BEGIN)

#ifndef NUI_ERROR_CODE_BEGIN
#define NUI_ERROR_CODE_BEGIN enum nui_error_code {
#define NUI_ERROR_CODE_ENTRY(NAME, STRNAME, DESC) NUI_ERROR_##NAME,
#define NUI_ERROR_CODE_END };
#endif

NUI_ERROR_CODE_BEGIN
NUI_ERROR_CODE_ENTRY(
		OK, 
		"Ok",
		"All is well."
)
NUI_ERROR_CODE_ENTRY(
		GENERIC, 
		"Generic",
		"Something went wrong")
NUI_ERROR_CODE_ENTRY(
		BUFFER_TOO_SMALL, 
		"Buffer too small",
		"The given buffer was too small. Pass a bigger one."
		)
NUI_ERROR_CODE_ENTRY(
		ENCODING, 
		"Encoding",
		"A string could not be encoded or decoded"
		)
NUI_ERROR_CODE_ENTRY(
		INVALID_ARGUMENT, 
		"Invalid argument",
		"An argument value violates a precondition"
		)
NUI_ERROR_CODE_ENTRY(
		FAILED, 
		"Failed",
		"A 'normal' operation failure to be expected & handled"
)
NUI_ERROR_CODE_END

#undef NUI_ERROR_CODE_FIRST
#undef NUI_ERROR_CODE_BEGIN
#undef NUI_ERROR_CODE_END
#undef NUI_ERROR_CODE_ENTRY

#endif
