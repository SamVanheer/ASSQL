#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLBIND_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLBIND_H

#include <cstdint>
#include <vector>

#include <mysql.h>

class CASMySQLBind final
{
public:
	CASMySQLBind() = default;
	~CASMySQLBind();

	void Set( enum_field_types type, MYSQL_BIND* pBind, const char* const pBuffer = nullptr, const size_t uiLength = 0 );

	void SetOutput( const MYSQL_FIELD& field, MYSQL_BIND* pBind );

	void Clear();

	MYSQL_BIND* m_pBind = nullptr;

	my_bool m_bIsNull = false;
	my_bool m_bError = false;

	union
	{
		int32_t		m_iVal32[ 2 ];
		int64_t		m_iVal64;

		uint32_t	m_uiVal32[ 2 ];
		uint64_t	m_uiVal64;

		float		m_flValue32[ 2 ];
		double		m_flValue64;
	};

	std::vector<char> m_Buffer;

private:
	CASMySQLBind( const CASMySQLBind& ) = delete;
	CASMySQLBind& operator=( const CASMySQLBind& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLBIND_H