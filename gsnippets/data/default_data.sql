INSERT INTO languages (id,name) VALUES (1,'C');
INSERT INTO languages (id,name) VALUES (2,'Python');
INSERT INTO languages (id,name) VALUES (3,'SQL');

INSERT INTO mime_types (id_lang , name) VALUES (1,'text/x-c');
INSERT INTO mime_types (id_lang , name) VALUES (1,'text/x-csrc');
INSERT INTO mime_types (id_lang , name) VALUES (1,'image/x-xpixmap');
INSERT INTO mime_types (id_lang , name) VALUES (2,'text/x-python');
INSERT INTO mime_types (id_lang , name) VALUES (2,'application/x-python');
INSERT INTO mime_types (id_lang , name) VALUES (3,'text/x-sql');

INSERT INTO snippets (id,name,content,preview) VALUES (1,'for','for(i=0;i<10;i++)\n{\n\t\n}','prev\nfor(i=0;i<10;i++)\n{\n\t\n}');
INSERT INTO snippets (id,name,content,preview) VALUES (2,'while','while(TRUE)\n{\n\t\n}','prev\nwhile(TRUE)\n{\n\t\n}');

INSERT INTO snippets_langs (id_lang,id_snippet) VALUES (1 ,1);
INSERT INTO snippets_langs (id_lang,id_snippet) VALUES (1 ,2);

INSERT INTO control (1 ,'1.0');


