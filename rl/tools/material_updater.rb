=begin
Copyright (c) 2008 Daniel Wickert

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.  

 This script adds necessary import statements for switch Ogre 1.4 -> Ogre 1.6
 Only run it once on any given set of materials.
 If a base material's source file can't be determined,
 it is imported from __unkown_material_file__,
 so it is easy to spot missing files.

 This script expects a single base directory as argument,
 if none given process current directory
 call:
 > ruby material_upgrader.rb [base_directory]

=end

require 'find'

base = ARGV.empty? ? '.' : ARGV[0]

# key: derived material name, value: base material name.
derived_materials = Hash.new
# key: file name, value: Array of material names.
materials_per_file = Hash.new
# key: material name, value: file base name.
file_per_material = Hash.new("__unkown_material_file__")

# Find all materials, their base materials and store which material is defined where.
Find.find(base) do |path|
  if test(?d, path) :
    # Do not enter hidden directories like .svn
    if File.basename(path)[0] == ?. :
      Find.prune
    end
    puts "Entering " + path + "..."
  else
    # Actual file. Is it a material or program?
    if path =~ /\.(material|program)$/ :
      puts "   Parsing #{File.basename(path)}..."
      materials = Array.new
      File.open(path, "r+").each do |line|
        # For each material in the file store it in an array.
        if line =~ /^\s*material\s+([^:\s]*)/ :
          puts "      found material #$1."
          materials << $1
          file_per_material[$1] = File.basename(path)
        end
        # For each dervied material store material and its base material in a hash.
        if line =~ /^\s*material\s+([^:\s]*)?\s*:\s*([^:\s]*)?\s+$/ :
          puts "         it is derived from #$2."
          derived_materials[$1] = $2
        end
      end
      materials_per_file[path] = materials
    end
  end
end

materials_per_file.each do |file, materials|
  # does the file contain derived materials?
  imports = Array.new
  materials.each do |mat|
    if derived_materials.has_key? mat :
      # Prepare an import statement.
      basemat = derived_materials[mat]
      importfile = file_per_material[basemat]
      if importfile != File.basename(file) :
        imports << "import #{basemat} from #{importfile}\n"        
      end
    end
  end
  if not imports.empty? :
    imports.sort!
    imports.uniq!
    # Open file and add imports at the beginning.
    puts "in file #{file}:"
    imports.each {|line| puts "   #{line}"}
    File.open(file, File::RDWR) do |f|
      content = f.readlines
      f.pos = 0
      f.print imports
      f.print "\n"
      f.print content
      f.truncate f.pos
    end
  end
end



