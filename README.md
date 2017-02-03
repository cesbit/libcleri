C Left-Right Parser
===================

Related projects
----------------
- [pyleri](https://github.com/transceptor-technology/pyleri): Python parser (can export grammar to pyleri, cleri and jsleri)
- [jsleri](https://github.com/transceptor-technology/jsleri): JavaScript parser

Why Cleri?
-----------
Cleri is an easy-to-use parser created for SiriDB. We first used [lrparsing](http://lrparsing.sourceforge.net/doc/html/) and wrote [jsleri](https://github.com/transceptor-technology/jsleri) for auto-completion and suggestions in our web console. Later we found small issues in lrparsing and also had difficulties keeping the language the same in both projects. That is when we decided to create Pyleri which can export a created language to JavaScript. In the future we might implement a full C-implementation of SiriDB so we already created a C version for wich is [cleri](https://github.com/transceptor-technology/cleri) the result.

