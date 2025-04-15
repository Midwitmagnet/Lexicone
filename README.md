Downloads all federal statues and treaties in xml format, saves them at your specified location. Statutes are getting broken up in paragraphs and saved in sparql queries in ttl files
Problems:
- you need to separate the treaties, as these are largely unformatted. their number, under which their xmmls are saved begin with 0. - you need to remove these manually
- some statutes break out of the usual formating, using <span> to denote the nodes/text. these will show up largely emppty
