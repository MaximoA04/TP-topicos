#include "includes/string.h"
#include <stddef.h>
#include <stdio.h>
#include "string.h"
#define isMayus(c) ((c) >= 'A' && (c) <= 'Z')
#define isMinus(c) ((c) >= 'a' && (c) <= 'z')
#define aMayus(c) (((c) >= 'a' && (c) <= 'z') ? (c) - ('a' - 'A') : (c))
#define aMinus(c) (((c) >= 'A' && (c) <= 'Z') ? (c) + ('a' - 'A') : (c))
#define esLetra(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))

size_t lenString(const char* str)
{
	size_t len = 0;
	while (str[len] != '\0') {
		len++;
	}
	return len;
}
// Retorna 0 si son iguales, <0 si str1 < str2, >0 si str1 > str2
int cmpString(const char* str1, const char* str2)
{
	int len1 = lenString(str1);
	int len2 = lenString(str2);
	const char* s1 = str1;
	const char* s2 = str2;
	int diff = *s1 - *s2;
	if (len1 != len2) {
		return len1 - len2;
	}
	while (diff == 0 && *s1 != '\0') {
		s1++;
		s2++;
		diff = *s1 - *s2;
	}
	return diff;
}
int cmpNString(const char* str1, const char* str2, size_t n)
{
	const char* s1 = str1;
	const char* s2 = str2;
	size_t i = 0;

	while (i < n && *s1 != '\0' && *s2 != '\0') {
		if (*s1 != *s2) {
			return *s1 - *s2;
		}
		s1++;
		s2++;
		i++;
	}

	if (i < n) {
		return *s1 - *s2;
	}

	return 0;
}
char* cpyNString(char* dest, const char* src, size_t lim)
{
	char* cActualDest = dest;
	const char* cActualSrc = src;
	while (*cActualSrc != '\0' && (cActualDest - dest < lim)) {
		*cActualDest = *cActualSrc;
		cActualDest++;
		cActualSrc++;
	}
	*cActualDest = '\0'; // Asegura que la cadena destino termine con null
	return dest; // Retorna la longitud de la cadena copiada
}
char* cpyString(char* dest, const char* src)
{
	char* cActualDest = dest;
	const char* cActualSrc = src;
	while (*cActualSrc != '\0') {
		*cActualDest = *cActualSrc;
		cActualDest++;
		cActualSrc++;
	}
	*cActualDest = '\0'; // Asegura que la cadena destino termine con null
	return dest; // Retorna la longitud de la cadena copiada
}
char* catString(char* dest, const char* src, size_t lim)
{
	char* cActualDest = dest;
	const char* cActualSrc = src;
	while (*cActualDest != '\0') {
		cActualDest++;
	}
	while ((cActualDest - dest < lim) && (*cActualSrc != '\0')) {
		*cActualDest = *cActualSrc;
		cActualDest++;
		cActualSrc++;
	}
	*cActualDest = '\0';
	return dest; // Retorna la longitud de la cadena concatenada
}
char* buscarChar(const char* str, char c)
{
	char* cActual = (char*)str;
	while (*cActual != '\0') {
		if (*cActual == c) {
			return cActual;
		}
		cActual++;
	}
	return NULL; // Retorna NULL si no se encuentra el caracter
}
char* buscarSubString(const char* str, const char* sub)
{
	const char* cActualStr = str;
	const char* cActualSub = sub;
	while (*cActualStr != '\0') {
		if (*cActualStr == *cActualSub) {
			const char* tempStr = cActualStr;
			const char* tempSub = cActualSub;
			while (*tempSub != '\0' && *tempStr == *tempSub) {
				tempStr++;
				tempSub++;
			}
			if (*tempSub == '\0') {
				return (char*)cActualStr;
			}
		}
		cActualStr++;
	}
	return NULL; // Retorna NULL si no se encuentra la subcadena
}
char* buscarStringEnReversa(const char* str, const char* sub)
{
	const char* cActualStr = str + lenString(str) - 1;
	const char* cActualSub = sub + lenString(sub) - 1;
	while (cActualStr >= str) {
		if (*cActualStr == *cActualSub) {
			const char* tempStr = cActualStr;
			const char* tempSub = cActualSub;
			while (tempSub >= sub && *tempStr == *tempSub) {
				tempStr--;
				tempSub--;
			}
			if (tempSub < sub) {
				return (char*)cActualStr;
			}
		}
		cActualStr--;
	}
	return NULL; // Retorna NULL si no se encuentra la subcadena
}
char* buscarCharEnStringEnReversa(const char* str, char c)
{
  size_t len = lenString(str);
    if (len == 0)
      return NULL;

  const char *cActual = str + len - 1;
  while (cActual >= str) {
    if (*cActual == c)
      return (char*)cActual;
    cActual--;
  }
	return NULL; // Retorna NULL si no se encuentra el caracter
}
char* reemplazarCharEnString(char* str, const char buscar, const char reemplazar)
{
	char* cActual = str;
	while (*cActual != '\0') {
		if (*cActual == buscar) {
			*cActual = reemplazar;
		}
		cActual++;
	}
	return str; // Retorna la cadena con los caracteres reemplazados
}
char* normarlizarString(char* str)
{
	char* i = str;
	*i = aMayus(*i);
	i++;
	while (*i) {
		if (esLetra(*i)) {
			if (*(i - 1) == ' ')
				*i = aMayus(*i);
			else
				*i = aMinus(*i);
		}
		i++;
	}
	return str;
}
char* normalizarPrimerChar(char* str)
{
	char* i = str;
	*i = aMayus(*i);

	i++;
	while (*i) {
		if (esLetra(*i)) {
			*i = aMinus(*i);
		}
		i++;
	}
	return str;
}
char* removerCharEnString(char* str, char c)
{
	char* src = str, * dst = str;
	while (*src) {
		if (*src != c) {
      *dst = *src;
      dst++;
    }
    src++;
	}
	*dst = '\0';
	return str; // Retorna la cadena con los caracteres removidos
}
size_t lenArrayString(const char** arr)
{
	size_t len = 0;
	while (*arr != NULL) {
		len++;
		arr++;
	}
	return len;
}
bool includeString(const char* str, const char** arr, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		if (cmpString(str, arr[i]) == 0) {
			return true;
		}
	}
	return false;
}

