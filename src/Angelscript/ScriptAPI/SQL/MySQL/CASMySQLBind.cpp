#include "CASMySQLBind.h"

CASMySQLBind::~CASMySQLBind()
{
	Clear();
}

void CASMySQLBind::Set( enum_field_types type, MYSQL_BIND* pBind, const char* const pBuffer, const size_t uiLength )
{
	Clear();

	m_pBind = pBind;

	memset( m_pBind, 0, sizeof( MYSQL_BIND ) );

	m_pBind->buffer_type = type;
	m_pBind->buffer = &m_iVal64;
	m_pBind->length = &m_pBind->buffer_length;
	m_pBind->is_null = &m_bIsNull;
	m_pBind->error = &m_bError;

	m_pBind->buffer_length = 0;

	switch( type )
	{
	case MYSQL_TYPE_NULL:
		m_pBind->buffer_length = 1;
		break;

	case MYSQL_TYPE_BIT:
		m_pBind->buffer_length = 1;
		break;

	case MYSQL_TYPE_TINY:
		m_pBind->buffer_length = sizeof( int8_t );
		break;

	case MYSQL_TYPE_SHORT:
		m_pBind->buffer_length = sizeof( int16_t );
		break;

	case MYSQL_TYPE_LONG:
		m_pBind->buffer_length = sizeof( int32_t );
		break;

	case MYSQL_TYPE_LONGLONG:
		m_pBind->buffer_length = sizeof( int64_t );
		break;

	case MYSQL_TYPE_FLOAT:
		m_pBind->buffer_length = sizeof( float );
		break;

	case MYSQL_TYPE_DOUBLE:
		m_pBind->buffer_length = sizeof( double );
		break;

	case MYSQL_TYPE_NEWDATE:
	case MYSQL_TYPE_DATE:
	case MYSQL_TYPE_TIME:
	case MYSQL_TYPE_TIMESTAMP:
	case MYSQL_TYPE_DATETIME:
		m_pBind->buffer = &m_Time;
		break;

	default:
	case MYSQL_TYPE_DECIMAL:
	case MYSQL_TYPE_NEWDECIMAL:
	case MYSQL_TYPE_TINY_BLOB:
	case MYSQL_TYPE_MEDIUM_BLOB:
	case MYSQL_TYPE_LONG_BLOB:
	case MYSQL_TYPE_BLOB:
	case MYSQL_TYPE_ENUM:
	case MYSQL_TYPE_VARCHAR:
	case MYSQL_TYPE_VAR_STRING:
	case MYSQL_TYPE_STRING:
		{
			if( uiLength > 0 )
			{
				m_Buffer.resize( uiLength );

				m_pBind->buffer = m_Buffer.data();
				m_pBind->buffer_length = m_Buffer.size();

				if( pBuffer )
				{
					memcpy( m_Buffer.data(), pBuffer, uiLength );
				}
			}

			break;
		}
	}
}

void CASMySQLBind::SetOutput( const MYSQL_FIELD& field, MYSQL_BIND* pBind )
{
	Set( field.type, pBind, nullptr, field.max_length );
}

void CASMySQLBind::Clear()
{
	m_Buffer.clear();
	m_pBind = nullptr;

	memset( &m_Time, 0, sizeof( MYSQL_TIME ) );
}