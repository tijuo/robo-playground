#!/usr/bin/ruby

NUM_LAYERS  = 0   	# Number of hidden layers
NUM_NEURONS = 9		# Number of neurons/hidden layer
NUM_INPUTS  = 9		# Number of input neurons
NUM_OUTPUTS = 3		# Number of output neurons

NUM_DNAS = 10		# Default number of dnas to create

def generate_rand_dna(filename)
  f = File.open(filename, 'wb')

  max_hp, attack, defense, max_speed = (rand() * 512).to_i, (rand() * 32).to_i, \
                                       (rand() * 32).to_i, (rand() * 32).to_i
  red, green, blue = (rand()*256).to_i, (rand()*256).to_i, (rand()*256).to_i

  [blue, green, red, max_hp & 0xFF, (max_hp >> 8) | (attack << 1) | ((defense & 0x03)  << 6), \
   (defense >> 2) | (max_speed << 3)].pack('C'*6).each_byte do |b|
    f.putc(b)
  end

  NUM_LAYERS.times do
    NUM_NEURONS.times do
      [(rand() * ((1 << 8) - 1)).to_i].pack('C').each_byte do |b|
        f.putc(b) # threshold ( between -128 to 127 inclusive )
      end
    end
  end

  NUM_OUTPUTS.times do
    [(rand() * ((1 << 8) - 1)).to_i].pack('C').each_byte do |b|
      f.putc(b)
    end
  end

  if NUM_LAYERS > 0
    (NUM_NEURONS * (NUM_INPUTS+NUM_OUTPUTS+(NUM_LAYERS-2)) ).times do
      [(rand() * ((1 << 8) - 1)).to_i].pack('C').each_byte do |b|
        f.putc(b) # input weight( between -1 to 1 inclusive )
      end
    end
  else
    (NUM_INPUTS * NUM_OUTPUTS).times do
      [(rand() * ((1 << 8) - 1)).to_i].pack('C').each_byte do |b|
        f.putc(b) # input weight( between -1 to 1 inclusive )
      end
    end
  end
  f.close
end


def show_usage
  puts "Usage: #{ARGV[0]} [num_dnas]"
  exit 1
end

show_usage if ARGV.length > 1

num_times = ARGV.length == 0 ? NUM_DNAS : ARGV[0].to_i
num_times.times { |n| generate_rand_dna("robo_dna#{n}.dna") }
