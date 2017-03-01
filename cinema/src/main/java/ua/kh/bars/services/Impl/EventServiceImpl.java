package ua.kh.bars.services.Impl;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import ua.kh.bars.entities.Auditorium;
import ua.kh.bars.entities.Event;
import ua.kh.bars.repositories.AuditoriumRepository;
import ua.kh.bars.repositories.EventRepository;
import ua.kh.bars.services.EventService;

import java.util.Date;
import java.util.List;

/**
 * Created by green on 3/18/16.
 */

@Service
@Transactional
public class EventServiceImpl implements EventService {

    @Autowired
    private EventRepository eventRepository;

    @Autowired
    private AuditoriumRepository auditoriumRepository;

    public List<Event> findAll() {
        return eventRepository.findAll();
    }

    public void create(String name, Long auditoriumId, int price) {
        Event event = new Event();
        Auditorium auditorium = auditoriumRepository.findOne(auditoriumId);
        event.setName(name);
        event.setAuditorium(auditorium);
        event.setPrice(price);

        // TODO: implement all fields including dates
        // Skipped due to lack of time
        event.setDate(new Date());
        eventRepository.save(event);
    }

    public Event findOne(Long id) {
        return eventRepository.findOne(id);
    }

}
