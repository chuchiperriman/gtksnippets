/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gsnippets-db.c
 *
 *  Copyright (C) 2008 - ChuchiPerriman <chuchiperriman@gmail.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.

 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
  
#include "gsnippets-db.h"
#include "gsnippets-common.h"

#include <sqlite3.h>


struct _GSnippetsDbPrivate
{
	sqlite3 *db;
};

static GObjectClass* parent_class = NULL;

#define GSNIPPETS_DB_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), GSNIPPETS_TYPE_DB, GSnippetsDbPrivate))

#define SNIPPET_COL_ID 0
#define SNIPPET_COL_NAME 1
#define SNIPPET_COL_CONTENT 2
#define SNIPPET_COL_LANG 3

static void
db_prepare_stmt (GSnippetsDb *self, sqlite3_stmt **stmt, const gchar *sql) 
{
	gint		res;	
	const char	*left;

	res = sqlite3_prepare_v2 (self->priv->db, sql, -1, stmt, &left);
	if (SQLITE_OK != res)
		g_error ("Failure while preparing statement, (error=%d, %s) SQL: \"%s\"", res, sqlite3_errmsg(self->priv->db), sql);
}

static gboolean
db_table_exists (GSnippetsDb *self, const gchar *name)
{
	gchar		*sql;
	sqlite3_stmt	*stmt;
	gint		res;

	sql = sqlite3_mprintf ("SELECT COUNT(*) FROM sqlite_master WHERE type = 'table' AND name = '%s';", name);
	db_prepare_stmt (self,&stmt, sql);
	sqlite3_reset (stmt);
	sqlite3_step (stmt);
	res = sqlite3_column_int (stmt, 0);
	sqlite3_finalize (stmt);
	sqlite3_free (sql);
	return (1 == res);
}

static gint
db_exec (GSnippetsDb *self, const gchar *sql)
{
	gchar	*err;
	gint	res;
	
	res = sqlite3_exec (self->priv->db, sql, NULL, NULL, &err);
	sqlite3_free (err);
	
	return res;
}

static void
db_create_tables(GSnippetsDb *self)
{
	db_exec (self,"CREATE TABLE languages (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL);");
	db_exec (self,"CREATE TABLE snippets (id INTEGER PRIMARY KEY AUTOINCREMENT, lang_id INTEGER NOT NULL, name TEXT NOT NULL, content TEXT NOT NULL);");
	db_exec (self,"CREATE TABLE control (id INTEGER ,version TEXT);");
}

static void
db_insert_default_data(GSnippetsDb *self)
{
	db_exec (self,"INSERT INTO control (1 ,'1.0');");
}

static GSnippetsItem*
db_snippet_load_from_stmt(sqlite3_stmt	*stmt)
{
	GSnippetsItem* snippet = gsnippets_item_new_full(
				sqlite3_column_int(stmt,SNIPPET_COL_ID),
				(gchar*)sqlite3_column_text(stmt,SNIPPET_COL_NAME),
				(gchar*)sqlite3_column_text(stmt,SNIPPET_COL_CONTENT),
				sqlite3_column_int(stmt,SNIPPET_COL_LANG));
	return snippet;
}

static GSList*
db_snippets_load_from_sql(GSnippetsDb *self, const gchar* sql)
{
	sqlite3_stmt	*stmt;
	GSnippetsItem* 	snippet = NULL;
	GSList *list = NULL;

	db_prepare_stmt(self,&stmt,sql);
	
	while(sqlite3_step (stmt) == SQLITE_ROW)
	{
		snippet = db_snippet_load_from_stmt (stmt);
		list = g_slist_append(list,snippet);
	} 

	sqlite3_finalize (stmt);

	return list;
}

static void
gsnippets_db_init (GSnippetsDb *self)
{
	self->priv = GSNIPPETS_DB_GET_PRIVATE(self);
	self->priv->db = NULL;
}

static void
gsnippets_db_finalize (GObject *object)
{
	/*GSnippetsDb *self = GSNIPPETS_DB(object);*/
	/*TODO disconnect if the db is connected */
	G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gsnippets_db_class_init (GSnippetsDbClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	parent_class = G_OBJECT_CLASS (g_type_class_peek_parent (klass));

	object_class->finalize = gsnippets_db_finalize;

	g_type_class_add_private (object_class, sizeof(GSnippetsDbPrivate));
}

GType
gsnippets_db_get_type (void)
{
	static GType our_type = 0;

	if(our_type == 0)
	{
		static const GTypeInfo our_info =
		{
			sizeof (GSnippetsDbClass), /* class_size */
			(GBaseInitFunc) NULL, /* base_init */
			(GBaseFinalizeFunc) NULL, /* base_finalize */
			(GClassInitFunc) gsnippets_db_class_init, /* class_init */
			(GClassFinalizeFunc) NULL, /* class_finalize */
			NULL /* class_data */,
			sizeof (GSnippetsDb), /* instance_size */
			0, /* n_preallocs */
			(GInstanceInitFunc) gsnippets_db_init, /* instance_init */
			NULL /* value_table */  
		};

		our_type = g_type_register_static (G_TYPE_OBJECT, "GSnippetsDb",
		                                   &our_info, 0);
	}

	return our_type;
}

GSnippetsDb*
gsnippets_db_new(void)
{
	GSnippetsDb *self = GSNIPPETS_DB(g_object_new (GSNIPPETS_TYPE_DB, NULL));
	return self;
}

void
gsnippets_db_connect(GSnippetsDb *self)
{
	gchar* db_path = g_strdup(gsnippets_common_get_gsnippets_db_path());
	if (!sqlite3_open (db_path, &self->priv->db)) {
		g_debug ("Data base file %s was not found... Creating new one.", db_path);
	}
	
	if (!db_table_exists(self,"control"))
	{
		db_create_tables(self);
		db_insert_default_data(self);
	}
	g_free(db_path);
}

void
gsnippets_db_disconnect (GSnippetsDb *self) 
{

	if (FALSE == sqlite3_get_autocommit(self->priv->db))
		g_warning ("Fatal: DB not in auto-commit mode. This is a bug. Data may be lost!");
		
	if (SQLITE_OK != sqlite3_close (self->priv->db))
		g_warning ("DB close failed: %s", sqlite3_errmsg (self->priv->db));
	
	self->priv->db = NULL;
}

GSnippetsItem*
gsnippets_db_load(GSnippetsDb *self, gint id)
{
	sqlite3_stmt	*stmt;
	gchar *sql;
	GSnippetsItem* 	snippet = NULL;
	gint		res;

	sql = sqlite3_mprintf("SELECT id,name,content,lang_id from snippets WHERE id = %i",id);
	db_prepare_stmt(self,&stmt,sql);
	
	if (sqlite3_step (stmt) == SQLITE_ROW) {
		snippet = db_snippet_load_from_stmt (stmt);
		res = sqlite3_step (stmt);
	} else {
		g_debug("There isn't any snippet with id: %i",id);
	}

	sqlite3_free (sql);
	sqlite3_finalize (stmt);

	return snippet;
}

void
gsnippets_db_save(GSnippetsDb *self, GSnippetsItem* snippet)
{
	sqlite3_stmt *oStmtUpdate; 
	if (gsnippets_item_get_id(snippet)<0)
	{
		sqlite3_prepare_v2(self->priv->db,"INSERT INTO snippets (name,content,lang_id) VALUES (?,?,?);",-1,&oStmtUpdate,NULL);
		sqlite3_bind_text(oStmtUpdate,1,gsnippets_item_get_name(snippet),-1,NULL);
		sqlite3_bind_text(oStmtUpdate,2,gsnippets_item_get_content(snippet),-1,NULL);
		sqlite3_bind_int(oStmtUpdate,3,gsnippets_item_get_lang_id(snippet));
	        g_debug("Step: %i", sqlite3_step(oStmtUpdate)); 
	        sqlite3_reset(oStmtUpdate); 
	        sqlite3_finalize(oStmtUpdate);
		g_debug("Error: %s", sqlite3_errmsg(self->priv->db));
		
		gsnippets_item_set_id(snippet,sqlite3_last_insert_rowid(self->priv->db));
	}
	else
	{
		sqlite3_prepare_v2(self->priv->db,"UPDATE snippets SET name=?, content=?, lang_id=? WHERE id=?;",-1,&oStmtUpdate,NULL);
		sqlite3_bind_text(oStmtUpdate,1,gsnippets_item_get_name(snippet),-1,NULL);
		sqlite3_bind_text(oStmtUpdate,2,gsnippets_item_get_content(snippet),-1,NULL);
		sqlite3_bind_int(oStmtUpdate,3,gsnippets_item_get_lang_id(snippet));
		sqlite3_bind_int(oStmtUpdate,4,gsnippets_item_get_id(snippet));
	        g_debug("Step: %i", sqlite3_step(oStmtUpdate)); 
	        sqlite3_reset(oStmtUpdate); 
	        sqlite3_finalize(oStmtUpdate);
		g_debug("Error: %s", sqlite3_errmsg(self->priv->db));
	}
}

void
gsnippets_db_delete(GSnippetsDb *self, gint id)
{
	gint res;
	gchar *sql;
	sql = sqlite3_mprintf("DELETE FROM snippets WHERE id = %i",id);
	res = db_exec(self, sql);
	sqlite3_free(sql);
}

gint
gsnippets_db_lang_insert(GSnippetsDb *self, const gchar* lang_name)
{
	gint res;
	gchar *sql;
	sql = sqlite3_mprintf("INSERT INTO languages (name) VALUES ('%s');",
				lang_name);
	res = db_exec(self, sql);
	sqlite3_free(sql);
	
	return sqlite3_last_insert_rowid(self->priv->db);
}

gint
gsnippets_db_lang_get_id(GSnippetsDb *self, const gchar* lang_name)
{
	sqlite3_stmt	*stmt;
	gchar *sql;
	gint		res;

	sql = sqlite3_mprintf("SELECT id from languages WHERE name = '%s'",lang_name);
	db_prepare_stmt(self,&stmt,sql);
	
	if (sqlite3_step (stmt) == SQLITE_ROW) {
		res = sqlite3_column_int (stmt, 0);
	} else {
		res = -1;
	}

	sqlite3_free (sql);
	sqlite3_finalize (stmt);

	return res;
}

GSList*
gsnippets_db_get_all(GSnippetsDb *self)
{
	return db_snippets_load_from_sql(self,"SELECT id,name,content,lang_id from snippets ORDER BY name");
}

GSList*
gsnippets_db_get_by_lang_name(GSnippetsDb *self, const gchar* language_name)
{
	gchar *sql;
	GSList *list;
	
	g_return_val_if_fail(language_name!=NULL,NULL);
	
	sql = sqlite3_mprintf(
			"SELECT s.id,s.name,s.content,s.lang_id FROM"
				" snippets s, languages l WHERE"
				" s.lang_id = l.id AND"
				" l.name = '%s' GROUP BY s.id ORDER BY s.name;",
				language_name
				);
	list = db_snippets_load_from_sql(self,sql);
	sqlite3_free (sql);
	return list;
				
}

GSList*
gsnippets_db_get_by_lang_id(GSnippetsDb *self, gint lang_id)
{
	gchar *sql;
	GSList *list;
	
	sql = sqlite3_mprintf(
			"SELECT s.id,s.name,s.content,s.lang_id FROM"
				" snippets s WHERE s.lang_id = %i ORDER BY s.name",
				lang_id
				);
	list = db_snippets_load_from_sql(self,sql);
	sqlite3_free (sql);
	return list;
				
}





