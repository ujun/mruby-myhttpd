# -*- coding: utf-8 -*-

def gchist str

  db = SQLite3::Database.new('/Users/juchino/Library/Application Support/Google/Chrome/Default/History')

  result = Array.new()
  result.push("<html><body>")

  db.execute("select url, title from urls where title like '%" + str + "%'" + 'and title not like ""') do |row, column|
    result.push("<a href='" + row[0] + "'>" + row[1] + "</a><br />")
  end

  result.push("</body></html>")

  return result
end
