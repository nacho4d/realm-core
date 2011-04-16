#include "Table.h"
#include <assert.h>

const ColumnType Accessor::type = COLUMN_TYPE_INT;
const ColumnType AccessorBool::type = COLUMN_TYPE_BOOL;
const ColumnType AccessorString::type = COLUMN_TYPE_STRING;

Table::Table(const char* name) : m_name(name), m_size(0), m_columns(COLUMN_HASREFS), m_columnNames(COLUMN_NORMAL) {
}

Table::~Table() {
	m_columns.Destroy();
	m_columnNames.Destroy();

	// free cached columns
	for (size_t i = 0; i < m_cols.Size(); ++i) {
		ColumnBase* const column = (ColumnBase* const)m_cols.Get(i);
		delete(column);
	}
}

void Table::RegisterColumn(ColumnType type, const char* name) {
	switch (type) {
	case COLUMN_TYPE_INT:
	case COLUMN_TYPE_BOOL:
		{
			Column* newColumn = new Column(COLUMN_NORMAL);
			
			m_columnNames.Add((int)name);

			m_columns.Add((int)newColumn->GetRef());
			newColumn->SetParent(&m_columns, m_columns.Size()-1);

			m_cols.Add((int)newColumn);
		}
		break;
	case COLUMN_TYPE_STRING:
		{
			/*Column refs(COLUMN_NORMAL);
			Column lengths(COLUMN_NORMAL);
			
			m_columnNames.Add((int)name);

			const size_t pos = m_columns.Size();
			m_columns.Add((int)refs.GetRef());
			m_columns.Add((int)lengths.GetRef());
			refs.SetParent(&m_columns, pos);
			lengths.SetParent(&m_columns, pos+1);

			StringColumn* newColumn = new StringColumn(refs, lengths);
			m_cols.Add((int)newColumn);*/

			AdaptiveStringColumn* newColumn = new AdaptiveStringColumn();
			
			m_columnNames.Add((int)name);

			m_columns.Add((int)newColumn->GetRef());
			newColumn->SetParent(&m_columns, m_columns.Size()-1);

			m_cols.Add((int)newColumn);
		}
		break;
	default:
		assert(false);
	}
}

ColumnBase& Table::GetColumnBase(size_t ndx) {
	assert(ndx < m_cols.Size());
	return *(ColumnBase* const)m_cols.Get(ndx);
}

const ColumnBase& Table::GetColumnBase(size_t ndx) const {
	assert(ndx < m_cols.Size());
	return *(const ColumnBase* const)m_cols.Get(ndx);
}

Column& Table::GetColumn(size_t ndx) {
	ColumnBase& column = GetColumnBase(ndx);
	assert(column.IsIntColumn());
	return static_cast<Column&>(column);
}

const Column& Table::GetColumn(size_t ndx) const {
	const ColumnBase& column = GetColumnBase(ndx);
	assert(column.IsIntColumn());
	return static_cast<const Column&>(column);
}

AdaptiveStringColumn& Table::GetColumnString(size_t ndx) {
	ColumnBase& column = GetColumnBase(ndx);
	assert(column.IsStringColumn());
	return static_cast<AdaptiveStringColumn&>(column);
}

const AdaptiveStringColumn& Table::GetColumnString(size_t ndx) const {
	const ColumnBase& column = GetColumnBase(ndx);
	assert(column.IsStringColumn());
	return static_cast<const AdaptiveStringColumn&>(column);
}

size_t Table::AddRow() {
	const size_t count = m_cols.Size();
	for (size_t i = 0; i < count; ++i) {
		ColumnBase& column = GetColumnBase(i);
		column.Add();
	}

	return m_size++;
}

void Table::Clear() {
	const size_t count = m_cols.Size();
	for (size_t i = 0; i < count; ++i) {
		ColumnBase& column = GetColumnBase(i);
		column.Clear();
	}
	m_size = 0;
}

void Table::DeleteRow(size_t ndx) {
	assert(ndx < m_size);

	const size_t count = m_cols.Size();
	for (size_t i = 0; i < count; ++i) {
		ColumnBase& column = GetColumnBase(i);
		column.Delete(ndx);
	}
}

int Table::Get(size_t column_id, size_t ndx) const {
	assert(column_id < m_cols.Size());
	assert(ndx < m_size);

	const Column& column = GetColumn(column_id);
	return column.Get(ndx);
}

void Table::Set(size_t column_id, size_t ndx, int value) {
	assert(column_id < m_cols.Size());
	assert(ndx < m_size);

	Column& column = GetColumn(column_id);
	column.Set(ndx, value);
}

const char* Table::GetString(size_t column_id, size_t ndx) const {
	assert(column_id < m_columns.Size());
	assert(ndx < m_size);

	const AdaptiveStringColumn& column = GetColumnString(column_id);
	return column.Get(ndx);
}

void Table::SetString(size_t column_id, size_t ndx, const char* value) {
	assert(column_id < m_cols.Size());
	assert(ndx < m_size);

	AdaptiveStringColumn& column = GetColumnString(column_id);
	column.Set(ndx, value);
}