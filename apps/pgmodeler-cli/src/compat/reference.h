/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2024 - Raphael Araújo e Silva <raphael@pgmodeler.io>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

/**
\ingroup pgmodeler-cli
\namespace CompatNs
\class Reference
\brief This is a minimal portion of the legacy class Reference (pgModeler 1.1.0-alpha1 and below) that is used to
together with CompatNs::View to convert views in older models to the new format introduced by pgModeler 1.1.0-beta.
*/

#ifndef COMPAT_REFERENCE_H
#define COMPAT_REFERENCE_H

#include "coreglobal.h"
#include "simplecolumn.h"
#include "physicaltable.h"

namespace CompatNs {

	class Reference {
		private:
			//! \brief Stores the table used by the reference
			PhysicalTable *table;

			//! \brief Stores the column used by the reference
			Column *column;

			//! \brief Stores the expression that defines one reference
			QString expression,

			//! \brief Stores the alias to the expression or table
			alias,

			//! \brief Stores only the alias for the column
			column_alias,

			//! \brief Stores the alias for the reference. This text will be displayed when the view is being show in compact mode
			ref_alias;

			//! \brief Indicates if the expression is used as entire view definition
			bool is_def_expr;

			/*! \brief Stores the columns that the reference (when being an expression [is_def_expr]) generates.
			 * These columns are used when drawing a view and that has only on definition expression.
			 * By having columns, instead of drawing the expression as a column of the view, the ones in
			 * this vector are displayed */
			std::vector<SimpleColumn> columns;

			/*! \brief Stores the tables that the reference object is using within the expression which defines the view
			 * when is_def_expr is set. These tables are used to hint the user which tables the view is using. */
			std::vector<PhysicalTable *> ref_tables;

		public:
			//! \brief Constants used to define the reference type
			enum ReferType: unsigned {
				ReferColumn, //! \brief The reference is based on a table column
				ReferExpression //! \brief The reference is based on an expression
			};

			//! \brief Constants used on the view code generation
			enum SqlType: unsigned {
				SqlWhere=1,
				SqlSelect=2,
				SqlFrom=4,
				SqlEndExpr=8,
				SqlViewDef=16
			};

			Reference();

			//! \brief Creates a reference based on a table column
			Reference(PhysicalTable *table, Column *column, const QString &tab_alias, const QString &col_alias);

			//! \brief Creates a reference based on a expression
			Reference(const QString &expression, const QString &expr_alias);

			/*! \brief Changes the behavior of the expression. Calling this method will cause the
			reference to be used as the entire view SQL definition */
			void setDefinitionExpression(bool value);

			void addReferencedTable(PhysicalTable *ref_table);
			int getReferencedTableIndex(PhysicalTable *ref_table);
			void clearReferencedTables();
			std::vector<PhysicalTable *> getReferencedTables();

			//! \brief Gets the referenced table
			PhysicalTable *getTable();

			//! \brief Gets the referenced column
			Column *getColumn();

			//! \brief Returns the alias for the referenced column
			QString getColumnAlias();

			//! \brief Returs the reference for the table or expression
			QString getAlias();

			//! \brief Returns the expression for the reference
			QString getExpression();

			//! \brief Returns the reference typ (see REFER_??? constants)
			ReferType getReferenceType();

			void setReferenceAlias(const QString &alias);

			QString getReferenceAlias();

			//! \brief Returns the SQL code definition
			QString getSQLDefinition(SqlType sql_type);

			//! \brief Returns the XML code definition
			QString getXMLDefinition();

			//! \brief Indicates if the reference when used as expression defines the entire view (raw sql definition)
			bool isDefinitionExpression();

			//! \brief Adds a column as being generated by the reference (if it is an expression)
			void addColumn(const QString &name, PgSqlType type, const QString &alias);
			void addColumn(Column *col);

			//! \brief Clears the columns generated by the expression
			void removeColumns();

			//! \brief Returns the columns which the reference generates
			std::vector<SimpleColumn> getColumns();

			/*! \brief Compare the attributes of two references returning true
			 when they have the same values */
			bool operator == (Reference &refer);

			std::vector<BaseObject *> getDependencies(bool incl_indirect_deps);

	};

}

#endif