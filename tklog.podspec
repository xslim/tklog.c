Pod::Spec.new do |s|
  s.name             = "tklog"
  s.version          = "0.0.1"
  s.summary          = "A C & Obj-c logging library, inspired by LCL."
  s.description      = <<-DESC
                       A simple logging library inspired by Lib Component Logging.
                       * Dynamic components
                       * simple usage
                       DESC
  s.homepage         = "https://github.com/xslim/tklog.c"
  s.license          = 'MIT'
  s.author           = { "Taras Kalapun" => "t.kalapun@gmail.com" }
  s.source           = { :git => "https://github.com/xslim/tklog.c.git", :tag => s.version.to_s }

  s.platform     = :ios, '3.0'
  s.requires_arc = false

  s.source_files = ['tklog.h', 'tklog.c']
end
