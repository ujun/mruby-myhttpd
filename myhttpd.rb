# -*- coding: utf-8 -*-

def gchist str

  db = SQLite3::Database.new('/Users/juchino/Library/Application Support/Google/Chrome/Default/History')

  if str != ""
  ¦ result = Array.new()
  ¦ db.execute("select * from urls where title like '%" + str + "%'" + 'and title not like ""') do |row, column|
      result.push(row[2])
    end
    return result
  else
    result = Array.new()
    db.execute('select * from urls where title not like ""') do |row, column|
      result.push("* " + row[2])
    end
    return result.join("\n")
  end
end
