<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-US" lang="en-US">
<!-- git web interface version 1.9.1, (C) 2005-2006, Kay Sievers <kay.sievers@vrfy.org>, Christian Gierke -->
<!-- git core binaries version 1.9.1 -->
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8"/>
<meta name="generator" content="gitweb/1.9.1 git/1.9.1"/>
<meta name="robots" content="index, nofollow"/>
<title>Savannah Git Hosting - autoconf-archive.git/summary</title>
<link rel="stylesheet" type="text/css" href="/gitweb/static/gitweb.css"/>
<link rel="alternate" title="autoconf-archive.git - log - RSS feed" href="/gitweb/?p=autoconf-archive.git;a=rss" type="application/rss+xml" />
<link rel="alternate" title="autoconf-archive.git - log - RSS feed (no merges)" href="/gitweb/?p=autoconf-archive.git;a=rss;opt=--no-merges" type="application/rss+xml" />
<link rel="alternate" title="autoconf-archive.git - log - Atom feed" href="/gitweb/?p=autoconf-archive.git;a=atom" type="application/atom+xml" />
<link rel="alternate" title="autoconf-archive.git - log - Atom feed (no merges)" href="/gitweb/?p=autoconf-archive.git;a=atom;opt=--no-merges" type="application/atom+xml" />
<link rel="shortcut icon" href="/gitweb/static/git-favicon.png" type="image/png" />
</head>
<body>
<div class="page_header">
<a title="git homepage" href="http://git-scm.com/"><img width="72" alt="git" src="/gitweb/static/git-logo.png" class="logo" height="27" /></a><a href="/gitweb/">git@sv</a> / <a href="/gitweb/?p=autoconf-archive.git;a=summary">autoconf-archive.git</a> / summary
</div>
<form method="get" action="/gitweb/" enctype="application/x-www-form-urlencoded"><div class="search">
<input value="autoconf-archive.git" name="p" type="hidden" />
<input value="search" name="a" type="hidden" />
<input type="hidden" value="HEAD" name="h" />
<select name="st" >
<option selected="selected" value="commit">commit</option>
<option value="grep">grep</option>
<option value="author">author</option>
<option value="committer">committer</option>
<option value="pickaxe">pickaxe</option>
</select> <a title="search help" href="/gitweb/?p=autoconf-archive.git;a=search_help">?</a> search:
<input type="text" name="s"  />
<span title="Extended regular expression"><label><input type="checkbox" name="sr" value="1" />re</label></span></div>
</form>
<div class="page_nav">
summary | <a href="/gitweb/?p=autoconf-archive.git;a=shortlog">shortlog</a> | <a href="/gitweb/?p=autoconf-archive.git;a=log">log</a> | <a href="/gitweb/?p=autoconf-archive.git;a=commit;h=e9fe18747b1270a29e72dd93108ba3ac2c85fa29">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=commitdiff;h=e9fe18747b1270a29e72dd93108ba3ac2c85fa29">commitdiff</a> | <a href="/gitweb/?p=autoconf-archive.git;a=tree">tree</a><br/>
<br/>
</div>
<div class="title">&nbsp;</div>
<table class="projects_list">
<tr id="metadata_desc"><td>description</td><td>GNU Autoconf Archive</td></tr>
<tr id="metadata_lchange"><td>last change</td><td><span class="datetime">Wed, 5 Jul 2017 11:42:49 +0000</span> (13:42 +0200)</td></tr>
<tr class="metadata_url"><td>URL</td><td>git://git0.savannah.gnu.org/autoconf-archive.git</td></tr>
<tr class="metadata_url"><td></td><td>https://git0.savannah.gnu.org/r/autoconf-archive.git</td></tr>
<tr class="metadata_url"><td></td><td>ssh:  USER@git0.savannah.gnu.org:/srv/git/autoconf-archive.git</td></tr>
</table>
<div class="header">
<a class="title" href="/gitweb/?p=autoconf-archive.git;a=shortlog">shortlog</a>
</div>
<table class="shortlog">
<tr class="dark">
<td title="2017-07-05"><i>2 days ago</i></td>
<td class="author"><a class="list" title="Search for commits authored by Peter Simons" href="/gitweb/?p=autoconf-archive.git;a=search;s=Peter+Simons;st=author">Peter Simons</a></td><td><a class="list subject" title="Merge pull request #125 from kallewoof/ax-jni-crosscompile-fix" href="/gitweb/?p=autoconf-archive.git;a=commit;h=e9fe18747b1270a29e72dd93108ba3ac2c85fa29">Merge pull request #125 from kallewoof/ax-jni-crosscomp... </a> <span class="refs"> <span class="head" title="heads/master"><a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/heads/master">master</a></span></span></td>
<td class="link"><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=e9fe18747b1270a29e72dd93108ba3ac2c85fa29">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=commitdiff;h=e9fe18747b1270a29e72dd93108ba3ac2c85fa29">commitdiff</a> | <a href="/gitweb/?p=autoconf-archive.git;a=tree;h=e9fe18747b1270a29e72dd93108ba3ac2c85fa29;hb=e9fe18747b1270a29e72dd93108ba3ac2c85fa29">tree</a> | <a href="/gitweb/?p=autoconf-archive.git;a=snapshot;h=e9fe18747b1270a29e72dd93108ba3ac2c85fa29;sf=tgz" title="in format: tar.gz">snapshot</a></td>
</tr>
<tr class="light">
<td title="2017-07-05"><i>2 days ago</i></td>
<td class="author"><a class="list" href="/gitweb/?p=autoconf-archive.git;a=search;s=Karl-Johan+Alm;st=author" title="Search for commits authored by Karl-Johan Alm">Karl-Johan Alm</a></td><td><a class="list subject" title="AX_JNI_INCLUDE_DIR: Do not use AC_CHECK_FILE as it is unavailable on some systems." href="/gitweb/?p=autoconf-archive.git;a=commit;h=9276689d8a2708e83b01ecb1c1c3827a788a4c63">AX_JNI_INCLUDE_DIR: Do not use AC_CHECK_FILE as it... </a></td>
<td class="link"><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=9276689d8a2708e83b01ecb1c1c3827a788a4c63">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=commitdiff;h=9276689d8a2708e83b01ecb1c1c3827a788a4c63">commitdiff</a> | <a href="/gitweb/?p=autoconf-archive.git;a=tree;h=9276689d8a2708e83b01ecb1c1c3827a788a4c63;hb=9276689d8a2708e83b01ecb1c1c3827a788a4c63">tree</a> | <a href="/gitweb/?p=autoconf-archive.git;a=snapshot;h=9276689d8a2708e83b01ecb1c1c3827a788a4c63;sf=tgz" title="in format: tar.gz">snapshot</a></td>
</tr>
<tr class="dark">
<td title="2017-06-28"><i>9 days ago</i></td>
<td class="author"><a title="Search for commits authored by Peter Simons" href="/gitweb/?p=autoconf-archive.git;a=search;s=Peter+Simons;st=author" class="list">Peter Simons</a></td><td><a class="list subject" href="/gitweb/?p=autoconf-archive.git;a=commit;h=a4096843878e003017c5aed43ee37b17e241ef97" title="Merge pull request #124 from smcv/ax-python-devel-libs-ldflags">Merge pull request #124 from smcv/ax-python-devel-libs... </a></td>
<td class="link"><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=a4096843878e003017c5aed43ee37b17e241ef97">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=commitdiff;h=a4096843878e003017c5aed43ee37b17e241ef97">commitdiff</a> | <a href="/gitweb/?p=autoconf-archive.git;a=tree;h=a4096843878e003017c5aed43ee37b17e241ef97;hb=a4096843878e003017c5aed43ee37b17e241ef97">tree</a> | <a title="in format: tar.gz" href="/gitweb/?p=autoconf-archive.git;a=snapshot;h=a4096843878e003017c5aed43ee37b17e241ef97;sf=tgz">snapshot</a></td>
</tr>
<tr class="light">
<td title="2017-06-27"><i>10 days ago</i></td>
<td class="author"><a class="list" title="Search for commits authored by Simon McVittie" href="/gitweb/?p=autoconf-archive.git;a=search;s=Simon+McVittie;st=author">Simon McVittie</a></td><td><a class="list subject" href="/gitweb/?p=autoconf-archive.git;a=commit;h=eadfb64c4d93c68b7cb61ca960bf59b131aebaa7" title="AX_PYTHON_DEVEL: Swap values of PYTHON_EXTRA_LIBS, PYTHON_EXTRA_LDFLAGS">AX_PYTHON_DEVEL: Swap values of PYTHON_EXTRA_LIBS,... </a></td>
<td class="link"><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=eadfb64c4d93c68b7cb61ca960bf59b131aebaa7">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=commitdiff;h=eadfb64c4d93c68b7cb61ca960bf59b131aebaa7">commitdiff</a> | <a href="/gitweb/?p=autoconf-archive.git;a=tree;h=eadfb64c4d93c68b7cb61ca960bf59b131aebaa7;hb=eadfb64c4d93c68b7cb61ca960bf59b131aebaa7">tree</a> | <a title="in format: tar.gz" href="/gitweb/?p=autoconf-archive.git;a=snapshot;h=eadfb64c4d93c68b7cb61ca960bf59b131aebaa7;sf=tgz">snapshot</a></td>
</tr>
<tr class="dark">
<td title="3 weeks ago"><i>2017-06-16</i></td>
<td class="author"><a href="/gitweb/?p=autoconf-archive.git;a=search;s=Peter+Simons;st=author" title="Search for commits authored by Peter Simons" class="list">Peter Simons</a></td><td><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=9b4f7d37a4694be9915457cd239300a8aea48416" class="list subject">Merge pull request #123 from marv/ax_ruby_devel</a></td>
<td class="link"><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=9b4f7d37a4694be9915457cd239300a8aea48416">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=commitdiff;h=9b4f7d37a4694be9915457cd239300a8aea48416">commitdiff</a> | <a href="/gitweb/?p=autoconf-archive.git;a=tree;h=9b4f7d37a4694be9915457cd239300a8aea48416;hb=9b4f7d37a4694be9915457cd239300a8aea48416">tree</a> | <a title="in format: tar.gz" href="/gitweb/?p=autoconf-archive.git;a=snapshot;h=9b4f7d37a4694be9915457cd239300a8aea48416;sf=tgz">snapshot</a></td>
</tr>
<tr class="light">
<td title="3 weeks ago"><i>2017-06-15</i></td>
<td class="author"><a class="list" href="/gitweb/?p=autoconf-archive.git;a=search;s=Marvin+Schmidt;st=author" title="Search for commits authored by Marvin Schmidt">Marvin Schmidt</a></td><td><a class="list subject" href="/gitweb/?p=autoconf-archive.git;a=commit;h=80837955c1f6064c06c8c657d1205260ae58a49e" title="AX_RUBY_EXT: Use RbConfig instead of deprecated Config module">AX_RUBY_EXT: Use RbConfig instead of deprecated Config... </a></td>
<td class="link"><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=80837955c1f6064c06c8c657d1205260ae58a49e">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=commitdiff;h=80837955c1f6064c06c8c657d1205260ae58a49e">commitdiff</a> | <a href="/gitweb/?p=autoconf-archive.git;a=tree;h=80837955c1f6064c06c8c657d1205260ae58a49e;hb=80837955c1f6064c06c8c657d1205260ae58a49e">tree</a> | <a href="/gitweb/?p=autoconf-archive.git;a=snapshot;h=80837955c1f6064c06c8c657d1205260ae58a49e;sf=tgz" title="in format: tar.gz">snapshot</a></td>
</tr>
<tr class="dark">
<td title="4 weeks ago"><i>2017-06-08</i></td>
<td class="author"><a class="list" title="Search for commits authored by Peter Simons" href="/gitweb/?p=autoconf-archive.git;a=search;s=Peter+Simons;st=author">Peter Simons</a></td><td><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=814eb433e5418b97b34c31ef927bf8e26a368f4a" title="Merge pull request #122 from SoapGentoo/ax-recursive-eval" class="list subject">Merge pull request #122 from SoapGentoo/ax-recursive... </a></td>
<td class="link"><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=814eb433e5418b97b34c31ef927bf8e26a368f4a">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=commitdiff;h=814eb433e5418b97b34c31ef927bf8e26a368f4a">commitdiff</a> | <a href="/gitweb/?p=autoconf-archive.git;a=tree;h=814eb433e5418b97b34c31ef927bf8e26a368f4a;hb=814eb433e5418b97b34c31ef927bf8e26a368f4a">tree</a> | <a href="/gitweb/?p=autoconf-archive.git;a=snapshot;h=814eb433e5418b97b34c31ef927bf8e26a368f4a;sf=tgz" title="in format: tar.gz">snapshot</a></td>
</tr>
<tr class="light">
<td title="4 weeks ago"><i>2017-06-05</i></td>
<td class="author"><a class="list" href="/gitweb/?p=autoconf-archive.git;a=search;s=David+Seifert;st=author" title="Search for commits authored by David Seifert">David Seifert</a></td><td><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=fb4f1ee20fcc1df13552bfc483be379900a5abfc" class="list subject">Move AX_RECURSIVE_EVAL to its own file</a></td>
<td class="link"><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=fb4f1ee20fcc1df13552bfc483be379900a5abfc">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=commitdiff;h=fb4f1ee20fcc1df13552bfc483be379900a5abfc">commitdiff</a> | <a href="/gitweb/?p=autoconf-archive.git;a=tree;h=fb4f1ee20fcc1df13552bfc483be379900a5abfc;hb=fb4f1ee20fcc1df13552bfc483be379900a5abfc">tree</a> | <a title="in format: tar.gz" href="/gitweb/?p=autoconf-archive.git;a=snapshot;h=fb4f1ee20fcc1df13552bfc483be379900a5abfc;sf=tgz">snapshot</a></td>
</tr>
<tr class="dark">
<td title="2 months ago"><i>2017-04-26</i></td>
<td class="author"><a class="list" title="Search for commits authored by Peter Simons" href="/gitweb/?p=autoconf-archive.git;a=search;s=Peter+Simons;st=author">Peter Simons</a></td><td><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=cfa14e9ac5d874abfc19e5d33eb7a1bbb9c4e899" class="list subject">Merge pull request #121 from velichkov/ax_code_coverage</a></td>
<td class="link"><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=cfa14e9ac5d874abfc19e5d33eb7a1bbb9c4e899">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=commitdiff;h=cfa14e9ac5d874abfc19e5d33eb7a1bbb9c4e899">commitdiff</a> | <a href="/gitweb/?p=autoconf-archive.git;a=tree;h=cfa14e9ac5d874abfc19e5d33eb7a1bbb9c4e899;hb=cfa14e9ac5d874abfc19e5d33eb7a1bbb9c4e899">tree</a> | <a href="/gitweb/?p=autoconf-archive.git;a=snapshot;h=cfa14e9ac5d874abfc19e5d33eb7a1bbb9c4e899;sf=tgz" title="in format: tar.gz">snapshot</a></td>
</tr>
<tr class="light">
<td title="2 months ago"><i>2017-04-25</i></td>
<td class="author"><a title="Search for commits authored by Vasil Velichkov" href="/gitweb/?p=autoconf-archive.git;a=search;s=Vasil+Velichkov;st=author" class="list">Vasil Velichkov</a></td><td><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=46098cc8b7f67ac976ca3a3c5a36e8e4e3417697" class="list subject">Fix initialization of CODE_COVERAGE_GENHTML_OPTIONS</a></td>
<td class="link"><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=46098cc8b7f67ac976ca3a3c5a36e8e4e3417697">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=commitdiff;h=46098cc8b7f67ac976ca3a3c5a36e8e4e3417697">commitdiff</a> | <a href="/gitweb/?p=autoconf-archive.git;a=tree;h=46098cc8b7f67ac976ca3a3c5a36e8e4e3417697;hb=46098cc8b7f67ac976ca3a3c5a36e8e4e3417697">tree</a> | <a href="/gitweb/?p=autoconf-archive.git;a=snapshot;h=46098cc8b7f67ac976ca3a3c5a36e8e4e3417697;sf=tgz" title="in format: tar.gz">snapshot</a></td>
</tr>
<tr class="dark">
<td title="2 months ago"><i>2017-04-21</i></td>
<td class="author"><a href="/gitweb/?p=autoconf-archive.git;a=search;s=Peter+Simons;st=author" title="Search for commits authored by Peter Simons" class="list">Peter Simons</a></td><td><a class="list subject" title="AX_SUBDIRS_CONFIGURE: fix documentation layout for proper HTML rendering" href="/gitweb/?p=autoconf-archive.git;a=commit;h=961dd75066bf2065c7cb2af02f6d4e835f067fdb">AX_SUBDIRS_CONFIGURE: fix documentation layout for... </a></td>
<td class="link"><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=961dd75066bf2065c7cb2af02f6d4e835f067fdb">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=commitdiff;h=961dd75066bf2065c7cb2af02f6d4e835f067fdb">commitdiff</a> | <a href="/gitweb/?p=autoconf-archive.git;a=tree;h=961dd75066bf2065c7cb2af02f6d4e835f067fdb;hb=961dd75066bf2065c7cb2af02f6d4e835f067fdb">tree</a> | <a title="in format: tar.gz" href="/gitweb/?p=autoconf-archive.git;a=snapshot;h=961dd75066bf2065c7cb2af02f6d4e835f067fdb;sf=tgz">snapshot</a></td>
</tr>
<tr class="light">
<td title="2 months ago"><i>2017-04-21</i></td>
<td class="author"><a class="list" title="Search for commits authored by Harenome Ranaivoarivony-Razanajato" href="/gitweb/?p=autoconf-archive.git;a=search;s=Harenome+Ranaivoarivony-Razanajato;st=author"><span title="Harenome Ranaivoarivony-Razanajato">Harenome Ranaivoariv... </span></a></td><td><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=2192c6c102c7e3c4e5cd69587ce7bf44ae9f728e" class="list subject">AX_SUBDIRS_CONFIGURE: new macro</a></td>
<td class="link"><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=2192c6c102c7e3c4e5cd69587ce7bf44ae9f728e">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=commitdiff;h=2192c6c102c7e3c4e5cd69587ce7bf44ae9f728e">commitdiff</a> | <a href="/gitweb/?p=autoconf-archive.git;a=tree;h=2192c6c102c7e3c4e5cd69587ce7bf44ae9f728e;hb=2192c6c102c7e3c4e5cd69587ce7bf44ae9f728e">tree</a> | <a title="in format: tar.gz" href="/gitweb/?p=autoconf-archive.git;a=snapshot;h=2192c6c102c7e3c4e5cd69587ce7bf44ae9f728e;sf=tgz">snapshot</a></td>
</tr>
<tr class="dark">
<td title="3 months ago"><i>2017-04-06</i></td>
<td class="author"><a href="/gitweb/?p=autoconf-archive.git;a=search;s=Peter+Simons;st=author" title="Search for commits authored by Peter Simons" class="list">Peter Simons</a></td><td><a title="AX_ELISP: replace unicode characters with simpler ASCII variants" href="/gitweb/?p=autoconf-archive.git;a=commit;h=da98cadae7ea5b64ad910e2e587316337b3fd20e" class="list subject">AX_ELISP: replace unicode characters with simpler ASCII... </a></td>
<td class="link"><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=da98cadae7ea5b64ad910e2e587316337b3fd20e">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=commitdiff;h=da98cadae7ea5b64ad910e2e587316337b3fd20e">commitdiff</a> | <a href="/gitweb/?p=autoconf-archive.git;a=tree;h=da98cadae7ea5b64ad910e2e587316337b3fd20e;hb=da98cadae7ea5b64ad910e2e587316337b3fd20e">tree</a> | <a href="/gitweb/?p=autoconf-archive.git;a=snapshot;h=da98cadae7ea5b64ad910e2e587316337b3fd20e;sf=tgz" title="in format: tar.gz">snapshot</a></td>
</tr>
<tr class="light">
<td title="3 months ago"><i>2017-04-06</i></td>
<td class="author"><a class="list" href="/gitweb/?p=autoconf-archive.git;a=search;s=Peter+Simons;st=author" title="Search for commits authored by Peter Simons">Peter Simons</a></td><td><a title="AX_PROG_EMACS: replace unicode characters with simpler ASCII variants" href="/gitweb/?p=autoconf-archive.git;a=commit;h=db4b30b6edc8bcc7f6944f72bb55a0f7f3c619bd" class="list subject">AX_PROG_EMACS: replace unicode characters with simpler... </a></td>
<td class="link"><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=db4b30b6edc8bcc7f6944f72bb55a0f7f3c619bd">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=commitdiff;h=db4b30b6edc8bcc7f6944f72bb55a0f7f3c619bd">commitdiff</a> | <a href="/gitweb/?p=autoconf-archive.git;a=tree;h=db4b30b6edc8bcc7f6944f72bb55a0f7f3c619bd;hb=db4b30b6edc8bcc7f6944f72bb55a0f7f3c619bd">tree</a> | <a href="/gitweb/?p=autoconf-archive.git;a=snapshot;h=db4b30b6edc8bcc7f6944f72bb55a0f7f3c619bd;sf=tgz" title="in format: tar.gz">snapshot</a></td>
</tr>
<tr class="dark">
<td title="3 months ago"><i>2017-04-06</i></td>
<td class="author"><a class="list" href="/gitweb/?p=autoconf-archive.git;a=search;s=Thien-Thi+Nguyen;st=author" title="Search for commits authored by Thien-Thi Nguyen">Thien-Thi Nguyen</a></td><td><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=7bf55a3d74562a5d8df6556b3d5db346d2c2ef22" class="list subject">AX_ELISP: New submission</a></td>
<td class="link"><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=7bf55a3d74562a5d8df6556b3d5db346d2c2ef22">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=commitdiff;h=7bf55a3d74562a5d8df6556b3d5db346d2c2ef22">commitdiff</a> | <a href="/gitweb/?p=autoconf-archive.git;a=tree;h=7bf55a3d74562a5d8df6556b3d5db346d2c2ef22;hb=7bf55a3d74562a5d8df6556b3d5db346d2c2ef22">tree</a> | <a href="/gitweb/?p=autoconf-archive.git;a=snapshot;h=7bf55a3d74562a5d8df6556b3d5db346d2c2ef22;sf=tgz" title="in format: tar.gz">snapshot</a></td>
</tr>
<tr class="light">
<td title="3 months ago"><i>2017-04-06</i></td>
<td class="author"><a class="list" title="Search for commits authored by Thien-Thi Nguyen" href="/gitweb/?p=autoconf-archive.git;a=search;s=Thien-Thi+Nguyen;st=author">Thien-Thi Nguyen</a></td><td><a class="list subject" href="/gitweb/?p=autoconf-archive.git;a=commit;h=356128066812c0ac60291ad2880676d527a4b42c">AX_PROG_EMACS: New submission</a></td>
<td class="link"><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=356128066812c0ac60291ad2880676d527a4b42c">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=commitdiff;h=356128066812c0ac60291ad2880676d527a4b42c">commitdiff</a> | <a href="/gitweb/?p=autoconf-archive.git;a=tree;h=356128066812c0ac60291ad2880676d527a4b42c;hb=356128066812c0ac60291ad2880676d527a4b42c">tree</a> | <a href="/gitweb/?p=autoconf-archive.git;a=snapshot;h=356128066812c0ac60291ad2880676d527a4b42c;sf=tgz" title="in format: tar.gz">snapshot</a></td>
</tr>
<tr>
<td colspan="4"><a href="/gitweb/?p=autoconf-archive.git;a=shortlog">...</a></td>
</tr>
</table>
<div class="header">
<a href="/gitweb/?p=autoconf-archive.git;a=tags" class="title">tags</a>
</div>
<table class="tags">
<tr class="dark">
<td><i>3 months ago</i></td>
<td><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=1c8255fd820ec8d6d11372475bacc7b3aa0985b1" class="list name">v2017.03.21</a></td>
<td><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=ddb550bad5bace35ab5bde9d5e577792c965f5ef" class="list subject">Autoconf Archive version 2017.03.21</a></td>
<td class="selflink"><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=ddb550bad5bace35ab5bde9d5e577792c965f5ef">tag</a></td>
<td class="link"> | <a href="/gitweb/?p=autoconf-archive.git;a=commit;h=1c8255fd820ec8d6d11372475bacc7b3aa0985b1">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/tags/v2017.03.21">shortlog</a> | <a href="/gitweb/?p=autoconf-archive.git;a=log;h=refs/tags/v2017.03.21">log</a></td>
</tr><tr class="light">
<td><i>9 months ago</i></td>
<td><a class="list name" href="/gitweb/?p=autoconf-archive.git;a=commit;h=d3d6bb4a4fb7df1508d1470d908dbd81b225a962">v2016.09.16</a></td>
<td><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=7db0dd29b027ced1f2524b656af981f7ab6d53aa" class="list subject">Autoconf Archive version 2016.09.16</a></td>
<td class="selflink"><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=7db0dd29b027ced1f2524b656af981f7ab6d53aa">tag</a></td>
<td class="link"> | <a href="/gitweb/?p=autoconf-archive.git;a=commit;h=d3d6bb4a4fb7df1508d1470d908dbd81b225a962">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/tags/v2016.09.16">shortlog</a> | <a href="/gitweb/?p=autoconf-archive.git;a=log;h=refs/tags/v2016.09.16">log</a></td>
</tr><tr class="dark">
<td><i>15 months ago</i></td>
<td><a class="list name" href="/gitweb/?p=autoconf-archive.git;a=commit;h=abd51ff8fbb0fa6995e860533cf5e7f60db181eb">v2016.03.20</a></td>
<td><a class="list subject" href="/gitweb/?p=autoconf-archive.git;a=tag;h=d64e21bddac63cb7c74c7fb501291cffe1656aef">Autoconf Archive version 2016.03.20</a></td>
<td class="selflink"><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=d64e21bddac63cb7c74c7fb501291cffe1656aef">tag</a></td>
<td class="link"> | <a href="/gitweb/?p=autoconf-archive.git;a=commit;h=abd51ff8fbb0fa6995e860533cf5e7f60db181eb">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/tags/v2016.03.20">shortlog</a> | <a href="/gitweb/?p=autoconf-archive.git;a=log;h=refs/tags/v2016.03.20">log</a></td>
</tr><tr class="light">
<td><i>21 months ago</i></td>
<td><a class="list name" href="/gitweb/?p=autoconf-archive.git;a=commit;h=557b34d93d5a03fd66a2d40b02152b8f12eccf4c">v2015.09.25</a></td>
<td><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=501f40aa459c64f2cb2ccf22b16e56d37d718b57" title="GNU Autoconf Archive Version 2015.09.25" class="list subject">GNU Autoconf Archive Version 2015... </a></td>
<td class="selflink"><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=501f40aa459c64f2cb2ccf22b16e56d37d718b57">tag</a></td>
<td class="link"> | <a href="/gitweb/?p=autoconf-archive.git;a=commit;h=557b34d93d5a03fd66a2d40b02152b8f12eccf4c">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/tags/v2015.09.25">shortlog</a> | <a href="/gitweb/?p=autoconf-archive.git;a=log;h=refs/tags/v2015.09.25">log</a></td>
</tr><tr class="dark">
<td><i>2 years ago</i></td>
<td><a class="list name" href="/gitweb/?p=autoconf-archive.git;a=commit;h=8c3a792472f405e3b89f9a2a48a700caa67089d8">v2015.02.24</a></td>
<td><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=927daaca51b7c8a67f5a1f0fbca104b8316e92fd" class="list subject">Autoconf Archive Version 2015.02.24</a></td>
<td class="selflink"><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=927daaca51b7c8a67f5a1f0fbca104b8316e92fd">tag</a></td>
<td class="link"> | <a href="/gitweb/?p=autoconf-archive.git;a=commit;h=8c3a792472f405e3b89f9a2a48a700caa67089d8">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/tags/v2015.02.24">shortlog</a> | <a href="/gitweb/?p=autoconf-archive.git;a=log;h=refs/tags/v2015.02.24">log</a></td>
</tr><tr class="light">
<td><i>2 years ago</i></td>
<td><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=6ddf085ffd6c699070fe61cdc09e76743a49875f" class="list name">v2015.02.04</a></td>
<td><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=06ab78be731e434efe39c11a91a70df7b527c386" class="list subject">Autoconf Archive Version 2015.02.04</a></td>
<td class="selflink"><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=06ab78be731e434efe39c11a91a70df7b527c386">tag</a></td>
<td class="link"> | <a href="/gitweb/?p=autoconf-archive.git;a=commit;h=6ddf085ffd6c699070fe61cdc09e76743a49875f">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/tags/v2015.02.04">shortlog</a> | <a href="/gitweb/?p=autoconf-archive.git;a=log;h=refs/tags/v2015.02.04">log</a></td>
</tr><tr class="dark">
<td><i>2 years ago</i></td>
<td><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=7f56aaabdac6dc45ee083c6f3afdfc412a600124" class="list name">v2014.10.15</a></td>
<td><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=34f91518355f1f2cba082678c60008a1ed8d009a" class="list subject">Autoconf Archive Version 2014.10.15</a></td>
<td class="selflink"><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=34f91518355f1f2cba082678c60008a1ed8d009a">tag</a></td>
<td class="link"> | <a href="/gitweb/?p=autoconf-archive.git;a=commit;h=7f56aaabdac6dc45ee083c6f3afdfc412a600124">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/tags/v2014.10.15">shortlog</a> | <a href="/gitweb/?p=autoconf-archive.git;a=log;h=refs/tags/v2014.10.15">log</a></td>
</tr><tr class="light">
<td><i>3 years ago</i></td>
<td><a class="list name" href="/gitweb/?p=autoconf-archive.git;a=commit;h=8935e464cac3a8eb584c093b35863ba08690471b">v2014.02.28</a></td>
<td><a title="GNU Autoconf Archive Version 2014.02.28" href="/gitweb/?p=autoconf-archive.git;a=tag;h=42b15b27650f65c19a707bee7db2f987ce69a0dd" class="list subject">GNU Autoconf Archive Version 2014... </a></td>
<td class="selflink"><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=42b15b27650f65c19a707bee7db2f987ce69a0dd">tag</a></td>
<td class="link"> | <a href="/gitweb/?p=autoconf-archive.git;a=commit;h=8935e464cac3a8eb584c093b35863ba08690471b">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/tags/v2014.02.28">shortlog</a> | <a href="/gitweb/?p=autoconf-archive.git;a=log;h=refs/tags/v2014.02.28">log</a></td>
</tr><tr class="dark">
<td><i>3 years ago</i></td>
<td><a class="list name" href="/gitweb/?p=autoconf-archive.git;a=commit;h=dca0d5c7de9729a45ebb8934db114912dc04e37b">v2013.11.01</a></td>
<td><a class="list subject" title="GNU Autoconf Archive Version 2013.11.01" href="/gitweb/?p=autoconf-archive.git;a=tag;h=7c9026664523434623f3cc26d9e82dee00686e86">GNU Autoconf Archive Version 2013... </a></td>
<td class="selflink"><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=7c9026664523434623f3cc26d9e82dee00686e86">tag</a></td>
<td class="link"> | <a href="/gitweb/?p=autoconf-archive.git;a=commit;h=dca0d5c7de9729a45ebb8934db114912dc04e37b">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/tags/v2013.11.01">shortlog</a> | <a href="/gitweb/?p=autoconf-archive.git;a=log;h=refs/tags/v2013.11.01">log</a></td>
</tr><tr class="light">
<td><i>4 years ago</i></td>
<td><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=f5f6d113d94f121210370e10f41045491581bdc2" class="list name">v2013.06.09</a></td>
<td><a class="list subject" title="GNU Autoconf Archive Version 2013.06.09" href="/gitweb/?p=autoconf-archive.git;a=tag;h=7167a25793d36ad25d7068c2625d4cad78a83b26">GNU Autoconf Archive Version 2013... </a></td>
<td class="selflink"><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=7167a25793d36ad25d7068c2625d4cad78a83b26">tag</a></td>
<td class="link"> | <a href="/gitweb/?p=autoconf-archive.git;a=commit;h=f5f6d113d94f121210370e10f41045491581bdc2">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/tags/v2013.06.09">shortlog</a> | <a href="/gitweb/?p=autoconf-archive.git;a=log;h=refs/tags/v2013.06.09">log</a></td>
</tr><tr class="dark">
<td><i>4 years ago</i></td>
<td><a class="list name" href="/gitweb/?p=autoconf-archive.git;a=commit;h=60e854d960fb96105bbe59449b21aa6a9d196de3">v2013.04.06</a></td>
<td><a class="list subject" title="GNU Autoconf Archive Version 2013.04.06" href="/gitweb/?p=autoconf-archive.git;a=tag;h=1d216b9587c50f9d6e59ad3bec8f8b0efd9242c5">GNU Autoconf Archive Version 2013... </a></td>
<td class="selflink"><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=1d216b9587c50f9d6e59ad3bec8f8b0efd9242c5">tag</a></td>
<td class="link"> | <a href="/gitweb/?p=autoconf-archive.git;a=commit;h=60e854d960fb96105bbe59449b21aa6a9d196de3">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/tags/v2013.04.06">shortlog</a> | <a href="/gitweb/?p=autoconf-archive.git;a=log;h=refs/tags/v2013.04.06">log</a></td>
</tr><tr class="light">
<td><i>4 years ago</i></td>
<td><a class="list name" href="/gitweb/?p=autoconf-archive.git;a=commit;h=39acabca5d882efa48dcc6954af87e34bc11f563">v2013.02.02</a></td>
<td><a class="list subject" title="GNU Autoconf Archive Version 2013.02.02" href="/gitweb/?p=autoconf-archive.git;a=tag;h=8a60961910acc03aef17518b92924aeadeee4f51">GNU Autoconf Archive Version 2013... </a></td>
<td class="selflink"><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=8a60961910acc03aef17518b92924aeadeee4f51">tag</a></td>
<td class="link"> | <a href="/gitweb/?p=autoconf-archive.git;a=commit;h=39acabca5d882efa48dcc6954af87e34bc11f563">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/tags/v2013.02.02">shortlog</a> | <a href="/gitweb/?p=autoconf-archive.git;a=log;h=refs/tags/v2013.02.02">log</a></td>
</tr><tr class="dark">
<td><i>4 years ago</i></td>
<td><a class="list name" href="/gitweb/?p=autoconf-archive.git;a=commit;h=46143062e93089a64550493e3a9659117559c662">v2012.11.14</a></td>
<td><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=627611bcad3d47691076ce5240760d44d41d95d2" title="GNU Autoconf Archive Version 2012.11.14" class="list subject">GNU Autoconf Archive Version 2012... </a></td>
<td class="selflink"><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=627611bcad3d47691076ce5240760d44d41d95d2">tag</a></td>
<td class="link"> | <a href="/gitweb/?p=autoconf-archive.git;a=commit;h=46143062e93089a64550493e3a9659117559c662">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/tags/v2012.11.14">shortlog</a> | <a href="/gitweb/?p=autoconf-archive.git;a=log;h=refs/tags/v2012.11.14">log</a></td>
</tr><tr class="light">
<td><i>4 years ago</i></td>
<td><a class="list name" href="/gitweb/?p=autoconf-archive.git;a=commit;h=6f4975c5cbfcf9c71852e75596b4b74f88c83195">v2012.09.08</a></td>
<td><a class="list subject" href="/gitweb/?p=autoconf-archive.git;a=tag;h=42f4f4d30a2c03df0ab991a4391cb336a1aa408f" title="GNU Autoconf Archive Version 2012.09.08">GNU Autoconf Archive Version 2012... </a></td>
<td class="selflink"><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=42f4f4d30a2c03df0ab991a4391cb336a1aa408f">tag</a></td>
<td class="link"> | <a href="/gitweb/?p=autoconf-archive.git;a=commit;h=6f4975c5cbfcf9c71852e75596b4b74f88c83195">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/tags/v2012.09.08">shortlog</a> | <a href="/gitweb/?p=autoconf-archive.git;a=log;h=refs/tags/v2012.09.08">log</a></td>
</tr><tr class="dark">
<td><i>5 years ago</i></td>
<td><a href="/gitweb/?p=autoconf-archive.git;a=commit;h=d94f9f602747b87f97912fa649144bed4339839d" class="list name">v2012.04.07</a></td>
<td><a class="list subject" title="GNU Autoconf Archive Version 2012.04.07" href="/gitweb/?p=autoconf-archive.git;a=tag;h=a2b0e3d1b50469dcc98d4c7f23933fb549effdac">GNU Autoconf Archive Version 2012... </a></td>
<td class="selflink"><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=a2b0e3d1b50469dcc98d4c7f23933fb549effdac">tag</a></td>
<td class="link"> | <a href="/gitweb/?p=autoconf-archive.git;a=commit;h=d94f9f602747b87f97912fa649144bed4339839d">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/tags/v2012.04.07">shortlog</a> | <a href="/gitweb/?p=autoconf-archive.git;a=log;h=refs/tags/v2012.04.07">log</a></td>
</tr><tr class="light">
<td><i>5 years ago</i></td>
<td><a class="list name" href="/gitweb/?p=autoconf-archive.git;a=commit;h=77bb2ab18a583eca3717406e47cba73426f59ab1">v2011.12.21</a></td>
<td><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=b57da13e32c04d5a800257ab1b3ca48cd7aa1871" title="GNU Autoconf Archive Version 2011.12.21" class="list subject">GNU Autoconf Archive Version 2011... </a></td>
<td class="selflink"><a href="/gitweb/?p=autoconf-archive.git;a=tag;h=b57da13e32c04d5a800257ab1b3ca48cd7aa1871">tag</a></td>
<td class="link"> | <a href="/gitweb/?p=autoconf-archive.git;a=commit;h=77bb2ab18a583eca3717406e47cba73426f59ab1">commit</a> | <a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/tags/v2011.12.21">shortlog</a> | <a href="/gitweb/?p=autoconf-archive.git;a=log;h=refs/tags/v2011.12.21">log</a></td>
</tr><tr>
<td colspan="5"><a href="/gitweb/?p=autoconf-archive.git;a=tags">...</a></td>
</tr>
</table>
<div class="header">
<a class="title" href="/gitweb/?p=autoconf-archive.git;a=heads">heads</a>
</div>
<table class="heads">
<tr class="dark">
<td><i>2 days ago</i></td>
<td class="current_head"><a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/heads/master" class="list name">master</a></td>
<td class="link"><a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/heads/master">shortlog</a> | <a href="/gitweb/?p=autoconf-archive.git;a=log;h=refs/heads/master">log</a> | <a href="/gitweb/?p=autoconf-archive.git;a=tree;h=refs/heads/master;hb=refs/heads/master">tree</a></td>
</tr><tr class="light">
<td><i>8 years ago</i></td>
<td><a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/heads/pre-savannah-history" class="list name">pre-savannah-history</a></td>
<td class="link"><a href="/gitweb/?p=autoconf-archive.git;a=shortlog;h=refs/heads/pre-savannah-history">shortlog</a> | <a href="/gitweb/?p=autoconf-archive.git;a=log;h=refs/heads/pre-savannah-history">log</a> | <a href="/gitweb/?p=autoconf-archive.git;a=tree;h=refs/heads/pre-savannah-history;hb=refs/heads/pre-savannah-history">tree</a></td>
</tr></table>
<div class="page_footer">
<div class="page_footer_text">GNU Autoconf Archive</div>
<a class="rss_logo" href="/gitweb/?p=autoconf-archive.git;a=rss" title="log RSS feed">RSS</a>
<a class="rss_logo" title="log Atom feed" href="/gitweb/?p=autoconf-archive.git;a=atom">Atom</a>
</div>
<script type="text/javascript" src="/gitweb/static/gitweb.js"></script>
<script type="text/javascript">
window.onload = function () {
	var tz_cookie = { name: 'gitweb_tz', expires: 14, path: '/' };
	onloadTZSetup('local', tz_cookie, 'datetime');
};
</script>
</body>
</html>