/* Select snippets by language */
select s.name from languages l ,snippets s, snippets_langs sl WHERE l.id = sl.id_lang AND s.id = sl.id_snippet AND l.name = "C" GROUP BY s.id;

/* Select snippets by mime-type */
select s.* from languages l ,snippets s, snippets_langs sl, mime_types mt WHERE l.id = sl.id_lang AND mt.id_lang = l.id AND mt.name = "text/x-c" GROUP BY s.id;


